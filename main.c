/* library defines */
#define PCRE2_CODE_UNIT_WIDTH 8
#define _BSD_SOURCE
#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

/* my defines */
#define MAX_LINK_LENGTH 200

// Standard libraries includes
#include <stdio.h>
#include <curl/curl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wait.h>
#include <fcntl.h>

// perl regex library
#include <pcre2posix.h>

// my includes
#include "linked_list.h"
#include "queue.h"
#include "main.h"

// Generic queue to hold link found
Queue * links; 

// CONSTANTS
int MAX_RESPONSE_LENGTH = 400000;
const int MAX_DEPTH = 3;



Response * initResponse(){
	Response * res = malloc(sizeof(Response));
	res->buffer = malloc(MAX_RESPONSE_LENGTH);
	res->offset = 0;
	res->size = MAX_RESPONSE_LENGTH;

	return res;

}


char * findLinks(char * data, size_t size, int level){

	// Copy the data to search and null terminate
	char * search = malloc(size+1);
	memcpy(search, data, size);
	search[size] = '\0';

	// The pattern of a link
	char * pattern = "href=\"(.+?)\"";

	// Match offsets will be stored in pmatch
	int matchSize = 3;
	regmatch_t pmatch[matchSize];

	// Store compiled regex
	regex_t preg;

	// Compile the regex
	int comp = regcomp(&preg, pattern, REG_EXTENDED | REG_NEWLINE);
	if(comp!=0){
		regfree(&preg);
		printf("comp failed");
		exit(-1);
	}

	int totalMatches = 0;
	int offset = 0;
	int lcounter = 0;
	int gindex = 1;
	Link * link = malloc(sizeof(Link));
	
	while(regexec(&preg, data, matchSize, pmatch, REG_NOTEOL)==0){

		// Save links to links queue
		int start;
		int end;
		int length;

		start =(int) pmatch[gindex].rm_so;
		end = (int) pmatch[gindex].rm_eo;
		length = end-start;

		if(end!=-1)
			offset = end;

		if(start==-1){
			break;

		}

		char * url = malloc(length+1);
		memcpy(url, &data[start], length);
		url[length] = '\0';
		link->url = url;
		link->level = level + 1;
		enq(links, (void *) link);
		lcounter++;
		totalMatches++;


		data = &data[offset];

	}

}

size_t cb(char * buffer, size_t itemSize, size_t nitems, void * res){

	size_t bytes = itemSize * nitems;
	
	// Check response size is within legit range otherwise realloc
	if(bytes > ((Response *) res)->size - ((Response *) res)->offset){
		Response * s = realloc(res, 2 * ((Response * ) res)->size);

		if(s==NULL){
			die("failed to allocate enough space for response");
		}

		res = s;
	}

	int offset = ((Response *) res)->offset;
	memcpy(&((Response *) res)->buffer[offset], buffer, bytes);
	((Response *) res)->offset += bytes;
	return bytes;

}

Link * createLink(){
	Link * link = malloc(sizeof(Link));
	link->level = 0;
	link->url = NULL;

	return link;
}

void printcb(void * p){
	printf("%s\n", (char *)p);
}

int main(int argc, char * argv[]){

	// Starting point for crawler
	char * seed;

	// set the seed with a user provided value or
	// with default if non provided
	if(argc == 1){
		seed = "https://neteacher.co.il";
	}else{
		seed = argv[1];
	}

	// Open file to write ouput
	FILE * fp = fopen("output.txt", "w");

	if(fp==NULL)
		die("file could not be opened!");

	// Initialize the links list
	links = initQueue(sizeof(Link));

	// Buffer to store response
	Response * res = initResponse();

	// Initialize the curl library
	CURL * curl = curl_easy_init();
	if(!curl){
		printf("curl_easy_init failed");
		exit(-1);
	}

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, res);

/*
	// Loop over queue and save links to file
	int length = links->size;
	for(int i=0; i < length; i++){
		Link * link = (Link *) deq(links); 
		fwrite(link->url, sizeof(char), strlen(link->url), fp);
		fwrite("\n", sizeof(char), 1, fp);
	}
*/
	/* Initial request and finding links for seed */
	// Send request for SEED
	curl_easy_setopt(curl, CURLOPT_URL, seed);
	CURLcode result = curl_easy_perform(curl);

	// Find the links in the response and enque them for SEED
	findLinks(res->buffer, res->offset, 0);

	// deq, request, search links in response untill queue is empty
	// MAX_DEPTH is used to limit depth of search
	while(links->size > 0){

		// Get the next link to send request to
		Link * currentLink = deq(links);
		
		// Write links into file
		fwrite(currentLink->url, sizeof(char), strlen(currentLink->url), fp);
		fwrite("\n", sizeof(char), 1, fp);

		//enum linkType type = checkLinkType(currentLink->url);

		if(currentLink->level >= MAX_DEPTH)
			continue;

		// Send request
		curl_easy_setopt(curl, CURLOPT_URL, currentLink->url);
		CURLcode result = curl_easy_perform(curl);

		if(result!=CURLE_OK){
			continue;
		}

		// Find the links in the response and enque them
		findLinks(res->buffer, res->offset, currentLink->level);

	}

	//printf("%s", res->buffer);
	/*
	   for(int i=0; i<nfiles; i++){
	   char sep[32];
	   snprintf(sep, sizeof(sep), "FILE %d START", i);
	   fwrite(sep, 1, strlen(sep), (FILE *)fp);
	// Set options
	curl_easy_setopt(curl, CURLOPT_URL, files[i]);
	CURLcode result = curl_easy_perform(curl);
	if(result!=CURLE_OK){
	printf("curl_easy_perform failed");
	exit(-1);
	}


	}
	 */
	fclose(fp);

	curl_easy_cleanup(curl);

	return 0;


}

void die(char * msg){
	printf("%s", msg);
	exit(-1);
}
