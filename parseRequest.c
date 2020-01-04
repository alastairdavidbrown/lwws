#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "httpRequest.h"
#include "httpResponse.h"
#include "lwws.h"



int parseRequest(char * s, httpRequest * req)
{
	int iNoHeaders = 0;
	const char *dSpace = " ";
	const char *dNewLine = "\n";

	char * t = malloc(BIG);
	char * l = malloc(BIG);

	LOGGER(10,"Tokenising: %s\n",s);

	//strtok ammends the first paramter so pass a pointer (declaration was immutable char array)
	l = strtok(s, dNewLine);
	t = strtok(l, " ");
	req->method = (char*) malloc(strlen(t)+1);
	// EXIT_IF_NULL(req->method);
	sprintf(req->method, "%s", t);

	t = strtok(NULL, " ");
	req->resource  = (char*) malloc(strlen(t)+1);
	// EXIT_IF_NULL(req->resource);
	sprintf(req->resource, "%s", t);

	t = strtok(NULL," ");
	req->httpVersion = (char*) malloc(strlen(t)+1);
	// EXIT_IF_NULL(req->resource);
	sprintf(req->httpVersion, "%s", t);

	// Get the rest of the headers... but ignore them!
  while ((t = strtok(NULL, "\n")))
    iNoHeaders++;

  req->numHeaders = iNoHeaders;

  LOGGER (1, "verb:%s resource:%s httpVersion:%s\n",
    req->method,
    req->resource,
    req->httpVersion);

	return (1);
}
