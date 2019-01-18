#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lwws.h"

int parseRequest(char * s, httpRequest * req)
{

  LOGGER(9,"raw request: %s\n", s);

  int iNoHeaders = 0;
  char * l;
  char * t;
  char * r;
  char d[2] = " ";


  r = (char*) malloc(BIG*sizeof(char)); //raw
  l = (char*) malloc(BIG*sizeof(char)); //line
  t = (char*) malloc(BIG*sizeof(char)); //token

  strcpy(r,s); // first parameter to strtok is amended
  //l  = strtok(r, "\n");
  strcpy(l, "GET /index.html HTTP/1.1");
  LOGGER(9,"token(line): %s\n", l);

  t = strtok(l, d);
  LOGGER(9,"token(1): %s\n", t);
  req->method = (char*) malloc(strlen(t)+1);
  sprintf(req->method, "%s\n", t);
  LOGGER(9, "req->method (value):%s:\n", (void *) req->method);
  LOGGER(9, "req->method (pointer):%p:\n", (void *) &(req->method));


  t = strtok(NULL, d);
  LOGGER(9,"token(2): %s\n", t);
  req->resource  = (char*) malloc(strlen(t)+1);
  sprintf(req->resource,"%s\n", t);
  LOGGER(9, "req->resource (value):%s:\n", (void *) req->resource);
  LOGGER(9, "req->resource (pointer):%p:\n", (void *) &(req->resource));


  t = strtok(NULL, d);
  LOGGER(9,"token(3): %s\n", t);
  req->httpVersion = (char*) malloc(strlen(t)+1);
  sprintf(req->httpVersion, "%s\n", t);
  LOGGER(9, "req->httpVersion (value):%s:\n", (void *) req->httpVersion);
  LOGGER(9, "req->httpVersion (pointer):%p:\n", (void *) &(req->httpVersion));


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
	free(r);
	free(l);
	free(t);

  return (1);

}

int parseRequest2(char * s, httpRequest * req)
//int parseRequest2()
{
	LOGGER(5, "s:%s\n", s);
	const char * d = " ";
	char * t;

	//strtok ammends the first paramter so pass a pointer (declaration was immutable char array)
	printf("...parsing %s with delimiter :%s:\n", s, d);

	t = strtok(s, d);
	//sprintf(t, "GET\n");
	req->method = (char*) malloc(BIG);
  sprintf(req->method, "%s\n", t);
	printf("...call 1 %s\n", t);
	printf("req->method:%s\n", (char *) req->method);
	if(9<10){printf("%s:%s:%d:", getTime(), __FUNCTION__, 18); printf("req->method (value):%s:\n", (char *) req->method);};
	LOGGER(9, "req->method (value):%s:\n", (char *) req->method);
	LOGGER(9, "req->method (pointer):%p:\n", (char *) &(req->method));

	t = strtok(NULL, d);
	req->resource  = (char*) malloc(BIG);
	sprintf(req->resource, "%s\n", t);
	printf("...call 2 %s\n", t);
	LOGGER(9, "req->resource (value):%s:\n", (void *) req->resource);
	LOGGER(9, "req->resource (pointer):%p:\n", (void *) &(req->resource));

	t = strtok(NULL, d);
	req->httpVersion = (char*) malloc(BIG);
	sprintf(req->httpVersion, "%s\n", t);
	printf("...call 3 %s\n", t);
	printf("req->httpVersion:%s\n", (char *) req->httpVersion);
	LOGGER(9, "req->httpVersion (value):%s:\n", (void *) req->httpVersion);
	LOGGER(9, "req->httpVersion (pointer):%p:\n", (void *) &(req->httpVersion));
	free(t);
	return (1);

}
