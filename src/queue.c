#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int empty(struct queue_t * q) {
        if (q == NULL) return 1;
	return (q->size == 0);
}

void insertionSort(struct pcb_t * procArr[], int size){
	if(size == 0) return;
	int i, hole;
	for(i = 1; i < size; i++){
		hole = i;
		struct pcb_t *proc = procArr[i];
		while(hole > 0 && procArr[hole-1] && proc
		 && procArr[hole-1]->priority > proc->priority){
		 	procArr[hole] = procArr[hole-1];
		 	hole--;
		 }
		 procArr[hole] = proc;
	}
}

void bubble(struct pcb_t * arr[], int size){
	if(size == 0) return;
	int i, j;
	for(i = 0 ; i < size; i++){
		for(j = 0; j < size-1; j++){
			if(arr[j]->priority == arr[j+1]->priority){
				if(arr[j]->pid <= arr[j+1]->pid){
					continue;
				}else{
					struct pcb_t * swap = arr[j];
					arr[j] = arr[j+1];
					arr[j+1] = swap;
				}
				continue;
			}
			if(arr[j]->priority > arr[j+1]->priority){
				struct pcb_t * swap = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = swap;
			}
		}
	}

}
void enqueue(struct queue_t * q, struct pcb_t * proc) {
        /* TODO: put a new process to queue [q] */        
        if(q->size == 0 && proc != NULL){
        	q->size ++;
        	q->proc[q->size-1] = proc;
        	return;
        }
        if(q->proc[q->size] == NULL && proc != NULL){
        	q->proc[q->size] = proc;
        	q->size++;        	
        	bubble(q->proc, q->size);
        	return;
        }
		if(q->size < MAX_QUEUE_SIZE && proc != NULL){			
			q->proc[(q->size)] = proc;
			q->size++;			
			bubble(q->proc,q->size);
		}else{
			return;	
		}		 
}

struct pcb_t * dequeue(struct queue_t * q) {
        /* TODO: return a pcb whose prioprity is the highest
         * in the queue [q] and remember to remove it from q
         * */
    if(empty(q)) return NULL;
    if(q->size <= 0) 	return NULL;
    struct pcb_t *highPcb = q->proc[0];
    int i;
    for(i = 0; i < q->size-1; i++){
    	q->proc[i] = q->proc[i+1];
    }
    q->proc[q->size-1] = NULL;
    q->size --;
    return highPcb;    
}

