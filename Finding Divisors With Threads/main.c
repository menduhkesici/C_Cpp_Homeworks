#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include "queue.h"

// While doing this assignment, the solution code to
// the producer and consumer problem from the textbook
// "Modern Operating Systems" by Tanenbaum is used.

// It is modified to allow for multiple consumers as
// there are multiple worker threads in the assignment.

// Also, two critical sections are implemented, one for
// the access to the queue, one for the access to the
// terminal, which is used by the worker threads to print.

// data structure to hold thread arguments which is commonly
// used by all the worker and the generator threads
struct thread_args
{
    struct queue* ptr_Queue;        // pointer to the queue
    int num_of_randoms;             // the amount of the random numbers
    int range_of_randoms;           // the possible range of the random numbers
    int curr_num_of_randoms;        // to keep count of total QueueRemoval operations
    pthread_mutex_t the_mutex;      // mutex for the access to the queue
    pthread_mutex_t the_mutex_term; // mutex for the access to the terminal
    pthread_cond_t cond_g, cond_w;  // for condition signaling
};


// declarations of the generator and worker threads
void *generator(void *args);
void *worker(void *args);


int main(int argc, char *argv[])
{
    // random operations are seeded with the current time so that
    // different random numbers are obtained in each run
    srand(time(0));

    // default arguments
    int num_of_threads = 5;         // the number of worker threads
    int queue_size = 10;            // the maximum size of the queue
    int num_of_randoms = 20;        // the amount of the random numbers
    int range_of_randoms = 100;     // the possible range of the random numbers

    // parse the command-line arguments with the getopt() function
    int opt;
    while ((opt = getopt(argc, argv, ":t:q:r:m:")) != -1)
        switch(opt)
        {
            case 't':
                num_of_threads = atoi(optarg);
                break;
            case 'q':
                queue_size = atoi(optarg);
                break;
            case 'r':
                num_of_randoms = atoi(optarg);
                break;
            case 'm':
                range_of_randoms = atoi(optarg);
                break;
            case ':':
                fprintf(stderr, "Option -%c requires an operand\n", optopt);
                break;
            case '?':
                fprintf(stderr, "Unrecognized option: '-%c'\n", optopt);
        }

    // initialize the struct thread_args
    struct thread_args* ptr_args = (struct thread_args*)malloc(sizeof(struct thread_args));
    if (ptr_args == 0)
    {
        fprintf(stderr, "ERROR: Out of memory!\n");
        exit(1);
    }

    // initialize the queue and the other thread arguments
    ptr_args->ptr_Queue = QueueInitialize(queue_size);
    ptr_args->num_of_randoms = num_of_randoms;
    ptr_args->range_of_randoms = range_of_randoms;
    ptr_args->curr_num_of_randoms = 0;

    // define the threads
    pthread_t generator_thr;
    pthread_t* worker_thr = (pthread_t*)malloc(num_of_threads * sizeof(pthread_t));

    // initialize the mutexes and the condition signals
    pthread_mutex_init(&(ptr_args->the_mutex), 0);
    pthread_mutex_init(&(ptr_args->the_mutex_term), 0);
    pthread_cond_init(&(ptr_args->cond_w), 0);
    pthread_cond_init(&(ptr_args->cond_g), 0);

    // initialize the threads
    pthread_create(&generator_thr, 0, generator, ptr_args);
    for (int i = 0; i < num_of_threads; i++)
    {
        pthread_create(&worker_thr[i], 0, worker, ptr_args);
    }

    // wait for the threads to terminate
    pthread_join(generator_thr, 0);
    for (int i = 0; i < num_of_threads; i++)
    {
        pthread_join(worker_thr[i], 0);
    }

    // destroy the mutexes and the condition signals
    pthread_cond_destroy(&(ptr_args->cond_w));
    pthread_cond_destroy(&(ptr_args->cond_g));
    pthread_mutex_destroy(&(ptr_args->the_mutex));
    pthread_mutex_destroy(&(ptr_args->the_mutex_term));

    return 0;
}


// implementation of the generator thread
void *generator(void *args)
{
    struct thread_args* ptr_args = ((struct thread_args*)args);

    // loop for (num_of_randoms) argument
    for (int i = 0; i < ptr_args->num_of_randoms; i++)
    {
        pthread_mutex_lock(&(ptr_args->the_mutex));     // get exclusive access to the queue
        while (QueueFull(ptr_args->ptr_Queue))          // check the necessary conditions
            pthread_cond_wait(&(ptr_args->cond_g), &(ptr_args->the_mutex));
        // insert item to the queue
        QueueInsert(ptr_args->ptr_Queue, (rand() % ptr_args->range_of_randoms + 1));
        pthread_cond_signal(&(ptr_args->cond_w));       // wake up the worker threads
        pthread_mutex_unlock(&(ptr_args->the_mutex));   // release access to the queue
    }

    pthread_exit(NULL);
}


// implementation the worker thread
void *worker(void *args)
{
    struct thread_args* ptr_args = ((struct thread_args*)args);
    int number;

    // loop until max. number of QueueRemoval operations is reached
    while (ptr_args->curr_num_of_randoms < ptr_args->num_of_randoms)
    {
        pthread_mutex_lock(&(ptr_args->the_mutex));         // get exclusive access to the queue

        // check if max. number of QueueRemoval operations is reached
        if (ptr_args->curr_num_of_randoms == ptr_args->num_of_randoms)
        {
            pthread_mutex_unlock(&(ptr_args->the_mutex));   // release access to the queue
            break;
        }

        // increase total number of QueueRemoval operations
        ptr_args->curr_num_of_randoms += 1;

        while (QueueEmpty(ptr_args->ptr_Queue))             // check the necessary conditions
            pthread_cond_wait(&(ptr_args->cond_w), &(ptr_args->the_mutex));
        number = QueueRemove(ptr_args->ptr_Queue);          // remove item from the queue
        pthread_cond_signal(&(ptr_args->cond_g));           // wake up the generator thread
        pthread_mutex_unlock(&(ptr_args->the_mutex));       // release access to the queue

        // sleep for random amount (0.1 to 1 second with increments of 0.1 seconds)
        sleep((rand() % 10 + 1) * 0.1);

        pthread_mutex_lock(&(ptr_args->the_mutex_term));    // get exclusive access to the terminal

        // print the divisible number to the terminal
        printf("Thread ID: %lu, Number: %d, Divisors:", pthread_self(), number);
        for (int i = 1; i <= number; i++)
            if (number % i == 0)
                printf(" %d", i);
        printf("\n");

        pthread_mutex_unlock(&(ptr_args->the_mutex_term));  // release access to the terminal

    }

    pthread_exit(NULL);

}
