#ifndef IPC_H
#define IPC_H

#define QUEUE_MAX_SIZE 10

// A generic pointer queue
typedef struct
{
    void *messages[QUEUE_MAX_SIZE];
    int front, rear, size;
} MessageQueue;

// Initialize the queue
void initQueue(MessageQueue *q);

// Enqueue a heap‐allocated pointer (returns 0 on success)
int sendMessage(MessageQueue *q, void *msg);

// Dequeue into *outmsg (returns 0 on success)
int receiveMessage(MessageQueue *q, void **outmsg);

#endif
