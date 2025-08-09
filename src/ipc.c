#include "ipc.h"
#include <stdio.h>
#include "utils.h"

void initQueue(MessageQueue *q)
{
    q->front = q->rear = q->size = 0;
}

int sendMessage(MessageQueue *q, void *msg)
{
    if (q->size >= QUEUE_MAX_SIZE)
        return -1;
    q->messages[q->rear] = msg;
    q->rear = (q->rear + 1) % QUEUE_MAX_SIZE;
    q->size++;
    // debug: printf("[QUEUE] Enqueued, size=%d\n", q->size);
    return 0;
}

int receiveMessage(MessageQueue *q, void **outmsg)
{
    if (q->size == 0)
        return -1;
    *outmsg = q->messages[q->front];
    q->front = (q->front + 1) % QUEUE_MAX_SIZE;
    q->size--;
    // debug: printf("[QUEUE] Dequeued, size=%d\n", q->size);
    return 0;
}
