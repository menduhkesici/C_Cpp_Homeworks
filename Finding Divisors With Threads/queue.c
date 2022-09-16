#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// implementation of the QueueInitialize function
struct queue* QueueInitialize(int capacity)
{
    // allocate memory for the array with the specified size
    struct queue *ptr_Queue = NULL;
    ptr_Queue = (struct queue*)malloc(sizeof(struct queue));
    // show error if unsuccessful
    if (ptr_Queue == 0)
    {
        fprintf(stderr, "ERROR: Out of memory!\n");
        exit(1);
    }

    // allocate memory for the array with the specified size
    ptr_Queue->memory_loc = (int*)malloc(capacity * sizeof(int));
    // show error if unsuccessful
    if (ptr_Queue->memory_loc == 0)
    {
        fprintf(stderr, "ERROR: Out of memory!\n");
        exit(1);
    }

    // initialize the variables accordingly
    ptr_Queue->max_size = capacity;
    ptr_Queue->curr_size = 0;
    ptr_Queue->front_index = 0;
    ptr_Queue->rear_index = 0;
};

// implementation of the QueueInsert function
void QueueInsert(struct queue *ptr_Queue, int number)
{
    // show error if the queue is full
    if (QueueFull(ptr_Queue))
    {
        fprintf(stderr, "ERROR: Queue is full!\n");
        exit(1);
    }

    // insert the value into the rear of the array
    ptr_Queue->memory_loc[ptr_Queue->rear_index] = number;

    // change the rear_index and curr_size variables accordingly
    ptr_Queue->rear_index = (ptr_Queue->rear_index + 1) % ptr_Queue->max_size;
    ptr_Queue->curr_size += 1;
}

// implementation of the QueueRemove function
int QueueRemove(struct queue *ptr_Queue)
{
    // show error if the queue is empty
    if (QueueEmpty(ptr_Queue))
    {
        fprintf(stderr, "ERROR: Queue is empty!\n");
        exit(1);
    }

    // insert the value from the front of the array
    int temp = ptr_Queue->memory_loc[ptr_Queue->front_index];

    // change the front_index and curr_size variables accordingly
    ptr_Queue->front_index = (ptr_Queue->front_index + 1) % ptr_Queue->max_size;
    ptr_Queue->curr_size -= 1;

    return temp;
}

// implementation of the QueueEmpty function
bool QueueEmpty(struct queue *ptr_Queue)
{
    // return 1 if the queue is empty, 0 otherwise
    return (ptr_Queue->curr_size == 0);
}

// implementation of the QueueFull function
bool QueueFull(struct queue *ptr_Queue)
{
    // return 1 if the queue is full, 0 otherwise
    return (ptr_Queue->curr_size == ptr_Queue->max_size);
}

// implementation of the QueueDestroy function
void QueueDestroy(struct queue *ptr_Queue)
{
    // free the allocated spaces for the array and the struct
    free(ptr_Queue->memory_loc);
    free(ptr_Queue);
}

