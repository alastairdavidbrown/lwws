#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lwws.h"


int parseRequest(char * s, httpRequest * req)
{
	int iNoHeaders;
	const char *dSpace = " ";
	const char *dNewLine = "\n";

	char * t = malloc(BIG);
	char * l = malloc(BIG);


	//strtok ammends the first paramter so pass a pointer (declaration was immutable char array)
	l = strtok(s, dNewLine);
	t = strtok(l, " ");
	req->method = (char*) malloc(BIG);
  sprintf(req->method, "%s", t);

	t = strtok(NULL," ");
	req->resource  = (char*) malloc(BIG);
	sprintf(req->resource, "%s", t);

	t = strtok(NULL," ");
	req->httpVersion = (char*) malloc(BIG);
	sprintf(req->httpVersion, "%s", t);

	// Get the rest of the headers... but ignore them!
  while (t) {
    t = strtok(NULL, "\n");
    iNoHeaders++;
  }
  req->numHeaders = iNoHeaders;

  LOGGER (1, "verb:%s resource:%s httpVersion:%s\n",
    req->method,
    req->resource,
    req->httpVersion);

	free(t);
	free(l);
	return (1);
}
