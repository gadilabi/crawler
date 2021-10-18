// Data structures
typedef struct response{
	char * buffer;
	int offset;
	int size;
} Response;

enum linkType{
	ABSOLUTE, RELATIVE, RESOURCE, BASE
};

typedef struct link{
	char * url;
	char * base;
	int level;
} Link;

void die(char * msg);
Response * initResponse(void);
void findLinks(char * data, size_t size, int level, char * baseURL);
size_t cb(char * buffer, size_t itemSize, size_t nitems, void * res);
enum linkType checkLinkType(char * url);
char * getBase(char * link);
