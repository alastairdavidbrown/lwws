#include "httpRequest.h"
#include "httpResponse.h"
#include "lwws.h"


#include <stdio.h>
#include <stdlib.h>

httpRequest * initialiseHttpRequest()
{
    httpRequest * req = malloc (sizeof(httpRequest));;

    req->method = NULL;
    req->resource = NULL;
    req->httpVersion = NULL;
    req->numHeaders = 0;
    req->requestHeaders = NULL;
    return (req);
}

void dumpHttpRequest(httpRequest * req)
{
    LOGGER(10, "Dumping httpRequest: %p\n", req);
    LOGGER(10, "\treq->method: %p\n", req->method);
    LOGGER(10, "\t\t...value: %s\n", req->method);

    LOGGER(10, "\treq->resource: %p\n", req->resource);
    LOGGER(10, "\t\t...value: %s\n", req->resource);

    LOGGER(10, "\treq->httpVersion: %p\n", req->httpVersion);
    LOGGER(10, "\t\t...value: %s\n", req->httpVersion);

    LOGGER(10, "\treq->numHeaders: %ld\n", req->numHeaders);

    LOGGER(10, "\treq->requestHeaders: %p\n", req->requestHeaders);
    for (int i=0; i<req->numHeaders; i++)
      LOGGER(10, "\t\t...header %d: name %s: value: %s\n",
            i,
            req->requestHeaders[i].name,
            req->requestHeaders[i].value);
}


void freeHttpRequest(httpRequest * req)
{
      if (req->method)free(req->method);
      if (req->resource)free(req->resource);
      if (req->httpVersion)free(req->httpVersion);
      //if (req->requestHeaders)free(req->requestHeaders);
      return;
}
