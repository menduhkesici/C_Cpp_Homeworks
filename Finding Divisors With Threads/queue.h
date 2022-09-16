#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdbool.h>

// data structure for the struct 'queue'
// which includes the necessary variables
struct queue
{
    int *memory_loc;
    int max_size;
    int curr_size;
    int front_index;
    int rear_index;
};

// declaration of the QueueInitialize function
struct queue* QueueInitialize(int max_size_);

// declaration of the QueueInsert function
void QueueInsert(struct queue *ptr_Queue, int number);

// declaration of the QueueRemove function
int QueueRemove(struct queue *ptr_Queue);

// declaration of the QueueEmpty function
bool QueueEmpty(struct queue *ptr_Queue);

// declaration of the QueueFull function
bool QueueFull(struct queue *ptr_Queue);

// declaration of the QueueDestroy function
void QueueDestroy(struct queue *ptr_Queue);


#endif // _QUEUE_H_
