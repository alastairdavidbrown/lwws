#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#include "lwws.h"
#include "parseRequest.h"
#include "reportMemory.h"
#include "getType.h"

// Use Consts properly
// allocate and deallocate in the same place
// pass immutable when appropriate

int main(int argv, char ** argc)
{

  struct sockaddr_in name;
  int namelen = sizeof(name);

  //Create socket
  int iSocket = socket(AF_INET, SOCK_STREAM, 0);
  handleReturn("socket", iSocket);

  //bind it to an address
  name.sin_family = AF_INET;
  name.sin_port = htons (LISTEN_PORT);
  name.sin_addr.s_addr = htonl (INADDR_ANY);
  int iBind = bind (iSocket, (struct sockaddr *) &name, sizeof (name));
  handleReturn("bind", iBind);

  // Listen for incoming
  int iListen = listen(iSocket,5);
  handleReturn("listen", iListen);
  LOGGER(1, "listening on port %d...\n\n", LISTEN_PORT);


  while (1)
  {

    if (signal(SIGINT, sig_handler) == SIG_ERR)
      LOGGER(2, "WARNING: can't catch SIGINT\n", NULL);

    // Request Handler
    httpRequest * req = malloc (sizeof(httpRequest));
    httpResponse * res = malloc (sizeof(httpResponse));
    initialiseReqRes(req,res);

    int iAccept = accept(iSocket,(struct sockaddr *)  &name,  (socklen_t*)&namelen);
    handleReturn("accept", iAccept);
    LOGGER(1, "request on port %d\n",  name.sin_port);

    char * sr = malloc(VERYBIG+1); // Shonky
    size_t r;
    r = recv (iAccept, (void *) sr, VERYBIG, 0); /* DANGER */
    handleReturn("recv", r);
    int i = parseRequest(sr, req);

    int iGetRes = getResource(req->resource, res);
    int isetResourceType = setResourceType(req->resource, res);
    int iConstructRespose = constructResponse(req, res);

    int iSend = send (iAccept, res->response, (res->headerLength + res->contentLength + 1) , MSG_OOB);
    handleReturn("send", iSend);
    LOGGER(1, "...sending response return %d\n", iSend);

    // int iSend = send (iAccept, "HTTP/1.1 200\nContent-Type: text/html\nContent-Length: 62\n\n<html><head></head><body><h1>Spoof Response</h1></body></html>", (57 + 62 + 1) , MSG_OOB);
    // handleReturn("send", iSend);

    int iClose = close(iAccept);
    handleReturn("close", iClose);

    // Free memory
    int iFreeReqRes = freeReqRes(req, res);
    handleReturn("freeMemory", iFreeReqRes);
    free (req);
    free (res);
    //free(sr); shouldnt bargh but does... meory leak
  }

  return (true);
}

const char * getTime()
{
  time_t curtime;
  struct tm *loctime;
  curtime = time (NULL);	// Current Time
  loctime = localtime (&curtime); // in a local representtaion
  return (strtok(asctime (loctime), "\n")); // Strip off the new line
}

int handleReturn(char * name, int iReturn)
{
  if (iReturn < 0  )
  {
    LOGGER(1, "%s: return is non zero %d, errno is %d: %s\n", name, iReturn, errno, strerror(errno));
    exit (-1);
  }else{
    return iReturn;
  }
}

int getResource(char * resourceName, httpResponse * res)
{
  FILE * fp;
  char p[VERYBIG];
  size_t s;
  char ch;
  int i = 0;

  // Root location plus the requested resource
  strcpy (p,PUBLICLOCATION);
  strcat (p, resourceName);

  LOGGER(10, "request file :%s:\n", p);

  fp = fopen(p, "r");
  if (fp)
  {
    // Find how big the file is
    fseek(fp, 0L, SEEK_END);
    long sz = ftell(fp);
    LOGGER(5, "file is %ld bytes\n", sz);
    rewind(fp);

    // Allocate space and read it
    res->responseBody = (void *) malloc(sz);
    int s = fread(res->responseBody, 1, sz, fp);
    res->contentLength = sz;
    LOGGER(10, "... res->contentLength = %ld\n", res->contentLength);
    fclose (fp);

    // Set the repsonse status
    res->status = (char *) malloc(sizeof(RESP200STATUS)); // HURL
    memcpy(res->status, RESP200STATUS, sizeof(RESP200STATUS));
  }else{
    // Requested file does not exist
    LOGGER(10, "... can't find %s\n", p);
    res->responseBody = (char *) malloc(strlen(RESP404MESSAGE));
    res->status = (char *) malloc(sizeof(RESP404STATUS)); // HURL
    memcpy(res->status, RESP404STATUS, sizeof(RESP404STATUS));
  }
  return (1); //shonky never returns anything except 1 ?!?

}

// Worl out what the mime type is based on the extension, bit shonky
int setResourceType(char * p, httpResponse * res)
{
    LOGGER(10, "...getting type for %s length is %ld\n", p, strlen(p));
    const char * sTypeExtension = malloc(VERYBIG); // hurl (again)
    const char * sTypeDescription = malloc(VERYBIG);


    // find the first dot from the end.
    for(long l = strlen(p); l>=0 ; l--){
      if (p[l] == '.') {
        strcpy((char*) sTypeExtension, p+l+1);
      }
    }

    LOGGER(10, "...type is %s\n", sTypeExtension);
    sTypeDescription = getType(sTypeExtension);
    res->contentType = (char *) malloc(strlen(sTypeDescription) + 1);
    strcpy(res->contentType, sTypeDescription);
    free((char *) sTypeExtension);

    // This barghs but shouldnt memory leak somewhere!!
    //free(sTypeDescription);

    return (1);
}

// Construt the response
int constructResponse(httpRequest * req, httpResponse * res)
{
  res->response = (char *) malloc(VERYBIG); // just been sick
  // WARNING - super shonky response is one char less than the stringlength becuase of the reader function
  //res->headerLength = sprintf(res->response, "HTTP1.1 %s\nContent-Type: text/html\nContent-Length: %ld\n\n",  res->status, (long) (res->contentLength ));
  res->headerLength = sprintf(res->response, "HTTP/1.1 %s\nContent-Type: %s\nContent-Length: %ld\n\n",  res->status, (char *) res->contentType, (long) (res->contentLength ));
  memcpy(res->response+res->headerLength, res->responseBody, res->contentLength);
  return (1); // Super shonky never retrns anything other than 1
}

// Initialise the request and response
int initialiseReqRes(httpRequest * req, httpResponse * res)
{
    req = (httpRequest * ) malloc (sizeof (httpRequest));

    req->method = NULL;
    req->resource = NULL;
    req->httpVersion = NULL;
    req->numHeaders = 0;
    req->headers = NULL;

    res = (httpResponse * ) malloc (sizeof (httpResponse));
    res->headerLength = 0;
    res->contentType = NULL;
    res->contentLength = 0;
    res->responseHeader = NULL;
    res->responseBody = NULL;
    res->response = NULL;
    res->status = NULL;

    return (1);  // Super shinky always returns 1
}


// Free the memory allocated
int freeReqRes(httpRequest * req, httpResponse * res)
{
    if (res->status)free(res->status);
    if (res->responseBody)free(res->responseBody);
    if (res->response)free(res->response);
    return (1);  // Super shinky always returns 1
}

// Handle signals
void sig_handler(int signo)
{
  LOGGER(1, "received signal %d\n", signo);
  if (signo == SIGINT)
  {
    LOGGER(1, "... handling SIGINT, exiting...\n", NULL);
    exit (-1);
  }
}
