#define PCRE2_CODE_UNIT_WIDTH 8
#define _BSD_SOURCE
#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include <stdio.h>
#include <curl/curl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wait.h>
#include <fcntl.h>
#include <pcre2posix.h>
#include "myString.h"
#include "myArray.h"

char * links[50];

char * findLinks(char * data, size_t size){

	// Copy the data to search and null terminate
	char * search = malloc(size+1);
	memcpy(search, data, size);
	search[size] = '\0';

	// The pattern of a link
	char * pattern = "href=\"(.+?)\"";

	// Match offsets will be stored in pmatch
	regmatch_t pmatch[5];

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
	
	while(regexec(&preg, data, 5, pmatch, REG_NOTEOL)==0){

		// Save links to links array
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

		char * link = malloc(length+1);
		memcpy(link, &data[start], length);
		link[length] = '\0';
		links[lcounter] = link;
		lcounter++;
		totalMatches++;


		data = &data[offset];

	}

	links[totalMatches] = NULL;
}

size_t cb(char * buffer, size_t itemSize, size_t nitems, void * fp){

	static int counter = 0;
	size_t bytes = itemSize * nitems;
	//printf("chunk %d size: %zu\n", counter, bytes);
	findLinks(buffer, bytes);
	//fwrite(buffer, bytes, 1, (FILE *)fp);
	return bytes;

}

char * copyFile(char * file){
	// File to write output to
	FILE * fp = fopen("output.txt", "a");



}

int main(int argc, char * argv[]){

	// Starting point for crawler
	char * seed;

	// File to be read
	FILE * fp = fopen("output.txt", "a");

	char * str = "<a href=\"https:neteacher.co.il\"\n <a href=\"google.com\">";
	size_t strSize = strlen(str);
	findLinks(str, strSize);

	int counter = 0;
	while(links[counter]!=NULL){
		printf("%s\n", links[counter]);
		counter++;

	}

	// Copy file to buffer
	//char * buffer = copyFile("output.txt");


	/*
	   if(fp!=NULL){

	   while()
	   findLinks()

	   exit(0);
	   }
	 */
	if(fp == NULL){
		printf("File could not be opened");
		exit(-1);
	}

	/*
	   Setting fp and seed
	   if user did not provide them they will be set to default
	 */
	if(argc == 1){
		seed = "https://neteacher.co.il";
		//fp = fopen("output.txt", "w");
	}else if(argc==2){
		seed = argv[1];
		//fp = fopen("output.txt", "w");
	}else{
		seed = argv[1];
		//fp = fopen(argv[2], "w");
	}

	CURL * curl = curl_easy_init();
	if(!curl){
		printf("curl_easy_init failed");
		exit(-1);
	}

	int nfiles = argc-1;
	char ** files = &argv[0]+1;

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA,(void *) fp);

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

	// Send request
	fclose(fp);

	curl_easy_cleanup(curl);

	return 0;


}
