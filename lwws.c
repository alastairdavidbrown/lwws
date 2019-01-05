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

#define LOGLEVEL 10
#define BIG 1024	//Asking for trouble
#define VERYBIG 65635 // asking fro bigger trouble
#define PUBLICLOCATION "./public/"
#define DEFAULTROOTFILE "index.html"

#define LOGGER(l, fmt, ...) if(l<LOGLEVEL){printf("%s: ", getTime()); printf(fmt, __VA_ARGS__);}

int main(int argv, char ** argc)
{

  struct sockaddr_in name;
  int namelen = sizeof(name);

  int iSocket = socket(AF_INET, SOCK_STREAM, 0);

  handleReturn("socket", iSocket);

  name.sin_family = AF_INET;
  name.sin_port = htons (LISTEN_PORT);
  name.sin_addr.s_addr = htonl (INADDR_ANY);

  int iBind = bind (iSocket, (struct sockaddr *) &name, sizeof (name));
  handleReturn("bind", iBind);

  int iListen = listen(iSocket,5);
  handleReturn("listen", iListen);
  LOGGER(1, "listening on port %d...\n\n", LISTEN_PORT);


  while (1)
  {

    struct httpRequest req;
    struct httpResponse res;

    int iAccept = accept(iSocket,(struct sockaddr *)  &name,  (socklen_t*)&namelen);
    handleReturn("accept", iAccept);

    LOGGER(1, "request on port %d\n",  name.sin_port);
    char sr[VERYBIG]; // Shonky
    size_t r;
    r = recv (iAccept, (void *) &sr, VERYBIG, 0); /* DANGER */
    handleReturn("recv", r);

    int iParse = parseRequest(sr, &req);
    LOGGER(1,"parsed request method is %s:\n",req.method);
    res = constructResponse(req);

    LOGGER(1, "sending response...\n", NULL);
    LOGGER(1, "... response %s\n", res.response);
    LOGGER(1, "... responseBody %s\n", res.responseBody);
    LOGGER(1, "... length %ld\n", strlen(res.response));
    int iSend = send (iAccept, res.response, strlen(res.response), MSG_OOB);

    handleReturn("send", iSend);
    int iClose = close(iAccept);
    handleReturn("close", iClose);
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

struct httpResponse  getResource(char * r)
{
  FILE * fp;
  char p[BIG];
  size_t s;
  char ch;
  int i = 0;
  httpResponse res;

  strcpy (p,PUBLICLOCATION);
  strcat (p, r);
  LOGGER(10, "getResource: reading %s\n", p);


  fp = fopen(p, "r");
  if (fp)
  {
    res.responseBody = (char *) malloc(i+1);
    while((ch = fgetc(fp)) != EOF)
    {
      res.responseBody[i]= ch;
      i++;
      res.responseBody = realloc(res.responseBody, i+1);
    }
    fclose (fp);
  }
  LOGGER(5, "getResource: responseBody i is %d\n", i);
  LOGGER(5, "getResource: responseBody is %s\n", res.responseBody);
  return (res); //shonky

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

struct httpResponse constructResponse(struct httpRequest req)
{
  struct httpResponse res;

  LOGGER(10, "constructResponse: about to get content %s\n", req.resource);
  res = getResource(req.resource);
  LOGGER(10, "constructResponse: about to construct response from resource %s\n", req.resource);
  LOGGER(10, "constructResponse: about to construct response from responseBody %s\n", res.responseBody);
  LOGGER(10, "constructResponse: about to construct response lenght is %ld\n", strlen(res.responseBody));

  res.response = (char *) malloc(10230);
  // WARNING - super shonky response is one char less than the stringlength becuase of the reader function
  sprintf(res.response, "HTTP1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", (long) strlen(res.responseBody) - 1, res.responseBody);

  LOGGER(5, "constructed response %s\n", res.response);
  return(res);

}
