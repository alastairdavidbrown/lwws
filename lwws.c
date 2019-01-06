#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <unistd.h>

#include "lwws.h"

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

    // Request Handler
    httpRequest req;
    httpResponse res;
    int iAccept = accept(iSocket,(struct sockaddr *)  &name,  (socklen_t*)&namelen);
    handleReturn("accept", iAccept);
    LOGGER(1, "request on port %d\n",  name.sin_port);
    char sr[VERYBIG]; // Shonky
    size_t r;
    r = recv (iAccept, (void *) &sr, VERYBIG, 0); /* DANGER */
    handleReturn("recv", r);

    int iParse = parseRequest(sr, &req);
    LOGGER(1,"parsed request method is %s:\n",req.method);
    int iConstructRespose = constructResponse(req, &res);

    LOGGER(1, "sending response...\n", NULL);
    LOGGER(10, "... response %s\n", res.response);
    LOGGER(10, "... responseBody %s\n", res.responseBody);
    LOGGER(5, "... length %ld\n", strlen(res.response));
    int iSend = send (iAccept, res.response, strlen(res.response), MSG_OOB);

    handleReturn("send", iSend);
    int iClose = close(iAccept);
    handleReturn("close", iClose);

    // Free memory
    // int iFree = freeReqRes(&req, &res);
    // handleReturn("freeMemory", iFree);\

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
  LOGGER(1, "%s: return is %d, errno is %d\n", name, iReturn, errno);
  if (iReturn < 0  )
  {
    exit (-1);
  }else{
    return iReturn;
  }
}

int getResource(char * resourceName, httpResponse * res)
{
  FILE * fp;
  char p[BIG];
  size_t s;
  char ch;
  int i = 0;

  strcpy (p,PUBLICLOCATION);
  strcat (p, resourceName);
  LOGGER(5, "getResource: reading %s\n", p);


  fp = fopen(p, "r");
  if (fp)
  {
    fseek(fp, 0L, SEEK_END);
    long sz = ftell(fp);
    LOGGER(5, "getResource: file is %ld bytes\n", sz);
    rewind(fp);
    res->responseBody = (char *) malloc(sz);
    while((ch = fgetc(fp)) != EOF)
    {
      res->responseBody[i]= ch;
      i++;
    }
    res->contentLength = sz;
    fclose (fp);
  }
  LOGGER(5, "getResource: responseBody i is %d\n", i);
  LOGGER(10, "getResource: responseBody is %s\n", res->responseBody);
  return (1); //shonky never returns anything except 1 ?!?

}

int parseRequest(char * s, httpRequest * req)
{

  int iNoHeaders = 0;
  char * l = strtok(s, "\n");
  char * t;

  t = strtok(l, " ");
  req->method = (char*) malloc(strlen(s)+1);
  req->method = t;

  t = strtok(NULL, " ");
  req->resource  = (char*) malloc(strlen(s)+1);
  req->resource = t;

  t = strtok(NULL, " ");
  req->httpVersion = (char*) malloc(strlen(s)+1);
  req->httpVersion = t;

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
  return (1);
}

int constructResponse(struct httpRequest req, httpResponse * res)
{

  LOGGER(10, "constructResponse: about to get content %s\n", req.resource);
  int iGetResouce = getResource(req.resource, res);
  LOGGER(10, "constructResponse: about to construct response from resource %s\n", req.resource);
  LOGGER(10, "constructResponse: about to construct response from responseBody %s\n", res->responseBody);
  LOGGER(10, "constructResponse: about to construct response lenght is %ld\n", res->contentLength);

  res->response = (char *) malloc(10230);
  // WARNING - super shonky response is one char less than the stringlength becuase of the reader function
  sprintf(res->response, "HTTP1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", (long) strlen(res->responseBody) - 1, res->responseBody);

  LOGGER(10, "constructed response %s\n", res->response);
  return (1); // Super shonky never retrns anything other than 1

}

int initialiseReqRes(httpRequest * req, httpResponse * res)
{

    req->method = NULL;
    req->resource = NULL;
    req->httpVersion = NULL;

    res->responseBody = NULL;
    res->response = NULL;

    return (1);  // Super shinky always returns 1
}


int freeReqRes(httpRequest * req, httpResponse * res)
{

    LOGGER(5, "freeReqRes:1", NULL)
    if (req->method)free(req->method);
    if (req->resource)free(req->resource);
    if (req->httpVersion)free(req->httpVersion);

    if (res->responseBody)free(res->responseBody);
    if (res->response)free(res->response);

    return (1);  // Super shinky always returns 1
}
