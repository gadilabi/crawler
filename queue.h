typedef struct queue{
	int size;
	struct linkedList * q;
	int dataType;
} Queue;

struct queue * initQueue(int dataType);
void enq(struct queue * q, void * value);
void * deq(struct queue * q);
void printQueue(struct queue * q, void printCallback(void * value));
void deqAll(struct queue * q);
