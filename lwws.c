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
    httpRequest req;
    httpResponse res;
    LOGGER(9, "req address: %p: res address %p\n", (void *) &req, (void *) &res);
    //initialiseReqRes(&req,&res);

    int iAccept = accept(iSocket,(struct sockaddr *)  &name,  (socklen_t*)&namelen);
    handleReturn("accept", iAccept);
    LOGGER(1, "request on port %d\n",  name.sin_port);

    char * sr = malloc(VERYBIG+1); // Shonky
    size_t r;
    r = recv (iAccept, (void *) sr, VERYBIG, 0); /* DANGER */
    handleReturn("recv", r);
    int i = parseRequest(sr, &req);

    int iConstructRespose = constructResponse(req, &res);
    //int iConstructRespose = spoofResponse(req, &res);

    LOGGER(1, "sending response...\n", NULL);
    LOGGER(5, "... header length %ld\n", res.headerLength);
    LOGGER(5, "... content length %ld\n", res.contentLength);
    LOGGER(5, "... payload length %ld\n", res.headerLength + res.contentLength);
    LOGGER(10, "... response %s\n", res.response );

    int iSend = send (iAccept, res.response, (res.headerLength + res.contentLength + 1) , MSG_OOB);
    handleReturn("send", iSend);
    int iClose = close(iAccept);
    handleReturn("close", iClose);

    // Free memory
    int iFreeReqRes = freeReqRes(&req, &res);
    handleReturn("freeMemory", iFreeReqRes);
    free(sr);
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
  LOGGER(1, "%s: return is %d, errno is %d: %s\n", name, iReturn, errno, strerror(errno));
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
  char p[VERYBIG];
  size_t s;
  char ch;
  int i = 0;

  strcpy (p,PUBLICLOCATION);
  strcat (p, resourceName);

  LOGGER(10, "request file :%s:\n", p);

  //fp = fopen("public/index.html", "r");
  fp = fopen(p, "r");
  LOGGER(10, "fopen %p\n", fp);
  fclose (fp);

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
    LOGGER(10, "getResource: res->contentLength = %ld\n", res->contentLength);

    res->status = (char *) malloc(sizeof(RESP200STATUS)); // HURL
    res->status = RESP200STATUS;
    fclose (fp);
    FILE * dfp = fopen("Content.out", "rw+");
    int wret;
    if (dfp){
      wret =  fwrite(res->responseBody,1, res->contentLength, dfp);
      LOGGER (10, "write returns %d\n",wret);
      fclose(dfp);
      LOGGER(10, "Caching %ld bytes of content to disk\n", res->contentLength);
    }
  }else{
    LOGGER(10, "Cant find %s\n", p);
    res->responseBody = (char *) malloc(strlen(RESP404MESSAGE));
    res->status = (char *) malloc(sizeof(RESP404STATUS)); // HURL
    res->status = RESP404STATUS;
  }
  return (1); //shonky never returns anything except 1 ?!?

}

// Guaranteed valid response
int spoofResponse(struct httpRequest req, httpResponse * res)
{
  #define SPOOF_BODY_LEN 62

  #define SPOOF_HEADERS "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 62\n\n"
  #define SPOOF_BODY "<html><head></head><body><h1>Spoof Response</h1></body></html>"

  LOGGER(10, "spoofResponse: ******* Spoofing response, request is %s\n", req.resource);
  res->response = (char *) malloc(VERYBIG); // just been sick
  res->headerLength = sprintf(res->response, SPOOF_HEADERS);

  // WARNING - super shonky response is one char less than the stringlength becuase of the reader function
  //res->headerLength = sprintf(res->response, "HTTP1.1 %s\nContent-Type: text/html\nContent-Length: %ld\n\n",  res->status, (long) (res->contentLength ));
  res->headerLength = strlen(SPOOF_HEADERS);
  res->contentLength = SPOOF_BODY_LEN;

  memcpy(res->response+res->headerLength, SPOOF_BODY, SPOOF_BODY_LEN);
  LOGGER(10, "spoofResponse: ******* Spoofing response, response is %s\n", res->response);

  return (1); // Super shonky never retrns anything other than 1

}

int constructResponse(struct httpRequest req, httpResponse * res)
{

  LOGGER(10, "constructResponse: about to get content %s\n", req.resource);
  int iGetResouce = getResource(req.resource, res);
  res->response = (char *) malloc(VERYBIG); // just been sick
  // WARNING - super shonky response is one char less than the stringlength becuase of the reader function
  //res->headerLength = sprintf(res->response, "HTTP1.1 %s\nContent-Type: text/html\nContent-Length: %ld\n\n",  res->status, (long) (res->contentLength ));
  res->headerLength = sprintf(res->response, "HTTP/1.1 %s\nContent-Type: text/html\nContent-Length: %ld\n\n",  res->status, (long) (res->contentLength ));
  memcpy(res->response+res->headerLength, res->responseBody, res->contentLength);
  return (1); // Super shonky never retrns anything other than 1

}

int initialiseReqRes(httpRequest * req, httpResponse * res)
{

    req->method = NULL;
    req->resource = NULL;
    req->httpVersion = NULL;

    res->responseBody = NULL;
    res->response = NULL;
    res->status = NULL;

    return (1);  // Super shinky always returns 1
}


int freeReqRes(httpRequest * req, httpResponse * res)
{

    // if (req->resource)free(req->resource);
    // if (req->httpVersion)free(req->httpVersion);
    // if (res->responseBody)free(res->responseBody);
    // if (res->response)free(res->response);
    //
    // if (req->method)free(req->method);
    // if (req->resource)free(req->resource);
    // if (req->httpVersion)free(req->httpVersion);

    return (1);  // Super shinky always returns 1
}

void sig_handler(int signo)
{
  LOGGER(1, "received signal %d\n", signo);
  if (signo == SIGINT)
  {
    LOGGER(1, "... handling SIGINT, exiting...\n", NULL);
    exit (-1);
  }
}
