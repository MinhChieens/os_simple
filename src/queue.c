#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int empty(struct queue_t *q)
{
	if (q == NULL)
		return 1;
	return (q->size == 0);
}


void enqueue(struct queue_t *q, struct pcb_t *proc)
{
	/* TODO: put a new process to queue [q] */
	if (q->size == 0 && proc != NULL)
	{
		q->size++;
		q->proc[q->size - 1] = proc;
		return;
	}
	if (q->proc[q->size] == NULL && proc != NULL)
	{
		q->proc[q->size] = proc;
		q->size++;		
		return;
	}
	if (q->size < MAX_QUEUE_SIZE && proc != NULL)
	{
		q->proc[(q->size)] = proc;
		q->size++;		
	}
	else
	{
		return;
	}
}

struct pcb_t *dequeue(struct queue_t *q)
{
	/* TODO: return a pcb whose prioprity is the highest
	 * in the queue [q] and remember to remove it from q
	 * */
	if (empty(q))
		return NULL;
	if (q->size <= 0)
		return NULL;
	struct pcb_t *highPcb = q->proc[0];
	int i;
	for (i = 0; i < q->size - 1; i++)
	{
		q->proc[i] = q->proc[i + 1];
	}
	q->proc[q->size - 1] = NULL;
	q->size--;
	return highPcb;
}
