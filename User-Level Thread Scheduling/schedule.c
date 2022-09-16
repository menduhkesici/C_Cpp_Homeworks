#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <ucontext.h>

#define NUM_OF_THREADS	5
#define STACK_SIZE		4096

#define EMPTY_STATE		0
#define READY_STATE		1
#define RUNNING_STATE	2
#define FINISHED_STATE	3

// data structure for holding the thread info
struct ThreadInfo
{
	ucontext_t context; // holds the context info
	int state;          // holds the state of the thread
	int num_of_tickets; // holds the weight of the thread
};

// the array of the threads
struct ThreadInfo Thread_Array[NUM_OF_THREADS];

// variables holding the indexes of
// the current and previous threads
int prev_index, curr_index;

// indicates whether all arguments are created as threads
int final_reached;

// function declarations
void initializeThread();
int createThread();
void runThread();
void exitThread();
void scheduler(int sig);
void counter(int n, int i);
int find_gcd(int num1, int num2);

int main(int argc, char const *argv[])
{
	// the program should take at least one argument
	if (argc <= 1)
	{
		printf("The program should take at least one argument.\n");
		return(0);
	}

	// all arguments must be positive integers
	for (int i = 1; i < argc; i++)
		if (atoi(argv[i]) <= 0)
		{
			printf("The program should take positive integer arguments.\n");
			return(0);
		}

    // random operations are seeded with the current time so that
    // different random numbers are obtained in each run
    srand(time(0));

	// find the greatest common divisor and the sum of all the numbers
	int gcd, total;
	if (argc <= 2)
	{
		gcd = atoi(argv[1]);
		total = atoi(argv[1]);
	}
	else
	{
		gcd = find_gcd(atoi(argv[1]), atoi(argv[2]));
		total = atoi(argv[1]) + atoi(argv[2]);
		for (int i = 3; i < argc; i++)
		{
			gcd = find_gcd(gcd, atoi(argv[i]));
			total += atoi(argv[i]);
		}
	}

	// print the initial outputs to the terminal
	printf("\tshare:\n");
	for (int i = 1; i < argc; i++)
		printf("\t%d/%d", atoi(argv[i]) / gcd, total / gcd);
	printf("\n\n\tThreads:\n");
	for (int i = 1; i < argc; i++)
		printf("\tT%d", i);
	printf("\n");

	// initialize the thread structure
	initializeThread();

	// set up triggering for scheduler() function
	signal(SIGALRM, scheduler);

    // use arguments one by one to create threads
    int status;
	int i = 1;
	while (i < argc)
	{
	    // find the first empty location in the thread array
		int ThreadNum = 1;
		while (ThreadNum < NUM_OF_THREADS)
		{
			if (Thread_Array[ThreadNum].state == EMPTY_STATE)
				break;
			ThreadNum++;
		}

        // if there is no empty location in the thread array, trigger schedule() function
		if (ThreadNum == NUM_OF_THREADS)
			raise(SIGALRM);
        // if there is an empty location, create thread in the specified location
		else
		{
			status = createThread(ThreadNum, i, atoi(argv[i]), atoi(argv[i]) / gcd);
			if (status == -1)
				exit(EXIT_FAILURE);
			i++;
		}
	}

	// indicate that all arguments are used to create threads
	final_reached = 1;

	// infinite loop
	while(1)
        raise(SIGALRM);

	return (0);
}

// initializes all global data structures for the thread
void initializeThread()
{
	// in the first location, the context
	// for the main() function is reserved
	getcontext(&Thread_Array[0].context);
	Thread_Array[0].state = RUNNING_STATE;
	Thread_Array[0].num_of_tickets = 0;

	// other array locations are empty
	for(int i = 1; i < NUM_OF_THREADS; i++)
	{
		Thread_Array[i].state = EMPTY_STATE;
		Thread_Array[i].num_of_tickets = 0;
	}

	// initially current index is 0,
	// so the running thread is main()
	prev_index = 0;
	curr_index = 0;

	// final_reached being 0 indicates that there are
	// more arguments yet to be used to create threads
	final_reached = 0;

	return;
}

// creates a new thread
int createThread(int ThreadNum, int index, int number, int ticket_no)
{
	int status;
	status = getcontext(&Thread_Array[ThreadNum].context);

    // print error message if the system is unable to create the new thread
	if (status == -1)
	{
		fprintf(stderr, "Error while creating the new thread.\n");
		return (-1);
	}
	// if creating the new thread is successful, initialize the thread info accordingly
	else
	{
		Thread_Array[ThreadNum].context.uc_link = &Thread_Array[0].context;
		Thread_Array[ThreadNum].context.uc_stack.ss_sp = malloc(STACK_SIZE);
		Thread_Array[ThreadNum].context.uc_stack.ss_size = STACK_SIZE;
		Thread_Array[ThreadNum].state = READY_STATE;
		Thread_Array[ThreadNum].num_of_tickets = ticket_no;
		makecontext(&Thread_Array[ThreadNum].context, (void (*)(void))counter, 2, number, index);

		return (0);
	}
}

// switches control from one thread to another
void runThread(int new_index)
{
    // store the index of current context in prev_index
    // and change the curr_index to the index of next context
	prev_index = curr_index;
	curr_index = new_index;

    // activate the timer that triggers the context switching
	alarm(2);

	// swap to the new context
	Thread_Array[curr_index].state = RUNNING_STATE;
	swapcontext(&Thread_Array[prev_index].context, &Thread_Array[curr_index].context);
}

// removes the thread from the thread array
void exitThread(int index)
{
    // free the associated stack with the corresponding thread
    // and indicate its array location as empty
	free(Thread_Array[index].context.uc_stack.ss_sp);
	Thread_Array[index].context.uc_stack.ss_size = 0;
	Thread_Array[index].state = EMPTY_STATE;
	Thread_Array[index].num_of_tickets = 0;

	return;
}

// makes context switching using lottery scheduling with two seconds of intervals
void scheduler(int sig)
{
    // cancel the pending alarm timer
	alarm(0);

	// if the current context is finished, remove the thread
	if (Thread_Array[curr_index].state == FINISHED_STATE)
		exitThread(curr_index);
    // if it is not finished yet, indicate it as ready
	else
		Thread_Array[curr_index].state = READY_STATE;

    // remove the finished threads
	for (int i = 1; i < NUM_OF_THREADS; i++)
        if (Thread_Array[i].state == FINISHED_STATE)
            exitThread(curr_index);

    // if there are empty locations in the thread array,
    // switch to main() function to fill up the empty location
	if (final_reached == 0)
		for (int i = 1; i < NUM_OF_THREADS; i++)
			if (Thread_Array[i].state == EMPTY_STATE)
			{
				runThread(0);
				return;
			}

    // calculate the total number of tickets in the thread array
	int total_tickets = 0;
	for (int i = 1; i < NUM_OF_THREADS; i++)
		total_tickets += Thread_Array[i].num_of_tickets;

    // if there are no threads, switch to the main() function
	if (total_tickets == 0)
		runThread(0);
    // if there are threads, pick a random number and choose
    // the new context to execute in a weighed way
	else
	{
		int num = rand() % total_tickets;
		for (int i = 1; i < NUM_OF_THREADS; i++)
		{
			num -= Thread_Array[i].num_of_tickets;
			if (num < 0)
			{
				runThread(i);
				break;
			}
		}
	}

	return;
}

// prints integers from 1 to n with 1 second of intervals
void counter(int n, int i)
{
	for (int j = 1; j <= n; j++)
	{
		for (int k = 0; k < i; k++)
			printf("\t");
		printf("%d\n", j);
		sleep(1);
	}

	// after finishing, indicate the corresponding thread as finished
	Thread_Array[curr_index].state = FINISHED_STATE;

	return;
}

// returns the greatest common divisor of two numbers
int find_gcd(int num1, int num2)
{
	while(num1 != num2)
	{
        if(num1 > num2)
            num1 -= num2;
        else
            num2 -= num1;
	}
    return num1;
}
