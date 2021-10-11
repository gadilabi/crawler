struct node{
	struct node * next;
	void * value;
};

typedef struct linkedList{
	struct node * first;
	struct node * last;
	int size;
	int dataType;
} LinkedList ;

void freeLinkedList(struct linkedList * llp);
struct node * createNode(struct linkedList * llp, void * value);
void addLink(struct linkedList *  , void * value);
struct linkedList * createLinkedList(int dataType);
void * getValueAt(struct linkedList * , int position);
struct node * getLink(struct linkedList * , int position );
void printLinkedList(struct linkedList *, void printCallback(void * value));
void addLinkAt(struct linkedList * , int position , void *);
void removeLinkAt(struct linkedList * , int position, int freeDeep );
