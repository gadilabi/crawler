#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#include "queue.h"

struct queue * initQueue(int dataType){
	struct queue * q = (struct queue *) malloc(sizeof(struct queue));
	struct linkedList * llp = createLinkedList(dataType);

	q->size = 0;
	q->q = llp;
	q->dataType = dataType;

	return q;

}

void freeQueue(struct queue * pq){
	freeLinkedList(pq->q);
	free(pq);
}

void enq(struct queue * q, void * value){
	addLink(q->q, value);
	q->size++;
}

void * deq(struct queue * q){
	if(q->size == 0){
		return NULL;
	}

	void * served = getValueAt(q->q, 0);
	//void * servedCopy = malloc(sizeof(q->dataType));
	//memcpy(servedCopy, served, q->dataType);
	removeLinkAt(q->q, 0, 1);
	q->size--;

	return served;
	//return servedCopy;
}

void printQueue(struct queue * q, void printCallback(void * value)){

	int qSize = q->size;

	for(int i=0; i < qSize; i++){
		printCallback(getValueAt(q->q, i));
	}

	return;

}

/*
   void deqAll(struct queue * q){
   int qSize = q->size;
   for(int i=0; i < qSize; i++){
   int * served = deq(q);
   printf("%d\n", *served);

   }

   }
 */
/*
   void printInt(void * value){
   printf("%d\n", * (int *) value);

   }
 */

/*
int main(){
	struct queue * q = initQueue(sizeof(int));
	int a[] = {1,2,3,4,5};
	enq(q, &a[0]);
	deq(q);
	enq(q, &a[1]);
	enq(q, &a[2]);
	enq(q, &a[3]);
	enq(q, &a[4]);
	freeQueue(q);
	//printQueue(q, printInt);
	//deqAll(q);
	return 0;
}
*/
