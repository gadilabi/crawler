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
	void * servedCopy = malloc(q->dataType);
	memcpy(servedCopy, served, q->dataType);
	removeLinkAt(q->q, 0, 1);
	q->size--;

	return servedCopy;
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
	int size = 256;
	struct queue * q = initQueue(size);
	char * s1 = "1) short and sweet";
	char * s2 = "2) longer but not over the top";
	char * s3 = "3) going to almost overflow the mf";
	enq(q, s1);
	enq(q, s2);
	enq(q, s3);
	int length = q->size;
	for(int i=0; i < length; i++){
		char * s = (char *) deq(q);
		printf("%s", s);
	}
	//freeQueue(q);
	//printQueue(q, printInt);
	//deqAll(q);
	return 0;
}
*/
