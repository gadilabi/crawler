// Data structures
typedef struct response{
	char * buffer;
	int offset;
	int size;
} Response;

enum linkType{
	ABSOLUTE, RELATIVE, RESOURCE
};

typedef struct link{
	char * url;
	int level;
} Link;

void die(char * msg);
Response * initResponse(void);
char * findLinks(char * data, size_t size, int level);
size_t cb(char * buffer, size_t itemSize, size_t nitems, void * res);
