#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

struct linkedList * createLinkedList(int dataType){
	struct node * first = NULL;
	struct node * last = first;
	struct linkedList * ll = (struct linkedList *) malloc(sizeof(struct linkedList));

	ll->first = first;
	ll->last = last;
	ll->size = 0;
	ll->dataType = dataType;

	return ll;
}

struct node * createNode(struct linkedList * llp, void * value){

	struct node * newNode = (struct node *) malloc(sizeof(struct node));
	void * data;

	if(value!=NULL){
		data = malloc(llp->dataType);
		memcpy(data, value, llp->dataType);
	}else{
		data = NULL;
	}

	newNode->value = data;
	newNode->next = NULL;
	return newNode;
}

void addLink(struct linkedList * llp , void * value){
	struct node * newNode = createNode(llp, value);

	if(llp->size == 0){
		llp->first = newNode;
		llp->last = newNode;
		llp->size++;
	}else{
		struct node * lastNode = llp->last;
		lastNode->next = newNode;
		llp->last = newNode;
		llp->size++;
	}


}

void * getValueAt(struct linkedList * llp, int position){
	struct node * nextNode = llp->first;
	for(int i=0; i<position; i++){
		nextNode = nextNode->next;
	}

	return nextNode->value;
}

struct node * getLink(struct linkedList * llp, int position){
	struct node * nextNode = llp->first;
	for(int i=0; i<position; i++){
		nextNode = nextNode->next;
	}

	return nextNode;

}

void addLinkAt(struct linkedList * llp, int position, void * value){

	struct node * newNode = createNode(llp ,value);

	if(position==0){
		newNode->next = llp->first;
		llp->first = newNode;
		llp->size++;
		return;

	}

	if(position==llp->size){
		addLink(llp, value);
		llp->size++;
		return;
	}

	struct node * currentNode = getLink(llp, position-1);
	struct node * nextNode = currentNode->next;


	currentNode->next = newNode;
	newNode->next = nextNode;
	llp->size++;

}

void printLinkedList(struct linkedList * llp, void printCallback(void * data)){
	if(llp->size==0){
		printf("empty list");
		return;
	}

	struct node * nextNode = llp->first;
	int counter = 0;
	while(nextNode->next!=NULL){
		printCallback(nextNode->value);
		nextNode = nextNode->next;
		counter++;
	}
	printCallback(nextNode->value);
}
   void printInt(void * value){
   printf("%d\n", * (int *) value);

   }

void freeLinkedList(struct linkedList * llp){
	struct node * current = llp->first;
	struct node * next;
	while(current!=NULL){
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	free(llp);

}

void removeLinkAt(struct linkedList * llp, int position, int freeDeep){

	//no more links
	if(llp->size==0)
		return;

	//out of bounds
	if(position<0 || position>llp->size-1)
		return;

	//single link
	if(llp->size==1){
		struct node * garbageLink = llp->first;
		void * garbageValue = garbageLink->value;
		free(garbageLink);
		llp->first = NULL;
		llp->last = NULL;
		llp->size = 0;
		if(freeDeep==1)
			free(garbageValue);
		return;
	}

	//removing the first
	if(position==0){
		struct node * garbageLink = llp->first;
		void * garbageValue = garbageLink->value;
		llp->first = llp->first->next;
		free(garbageLink);
		if(freeDeep==1)
			free(garbageValue);
		llp->size--;
		return;
	}

	//removing the last
	if(position == llp->size-1){
		struct node * garbageLink = llp->last;
		void * garbageValue = garbageLink->value;
		struct node * beforeLast = getLink(llp, llp->size-2);
		llp->last = beforeLast;
		llp->last->next=NULL;
		free(garbageLink);
		if(freeDeep==1)
			free(garbageValue);
		llp->size--;
		return;

	}

	// removing at any other position
	struct node * garbageLink = getLink(llp, position);
	void * garbageValue = garbageLink->value;
	struct node * before = getLink(llp, position-1);
	struct node * after = getLink(llp, position+1);

	before->next = after;
	llp->size--;
	free(garbageLink);
	if(freeDeep==1)
		free(garbageValue);
}
/*
   struct linkedList * reverse(struct linkedList * llp){
   if(llp->size==0){
   return llp;
   }
   void * firstValue = llp->first->value;
   removeLinkAt(llp, 0);
   struct linkedList * rllp = reverse(llp);
   addLink(rllp, firstValue);
   return llp;
   }
   */

void printFloat(void * value){
	printf("%f\n", * (float *) value);

}

void printChar(void * c){
	printf("%c", * (char *) c);


}
/*
int main(){

	struct linkedList * llp = createLinkedList(sizeof(int));
	char * c = "hello world!";
	int i[] = {1,2,3,4};
	addLink(llp, &i[0]);
	//removeLinkAt(llp, 0, 0);
	addLink(llp, &i[1]);
	addLink(llp, &i[2]);
	addLink(llp, &i[3]);
	//printLinkedList(llp, printInt);
	freeLinkedList(llp);
	//reverse(llp);
	//printLinkedList(llp, printInt);

}
*/

