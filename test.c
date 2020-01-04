#include "httpRequest.h"
#include "httpResponse.h"
#include "lwws.h"
#include "parseRequest.h"

#include "stdlib.h"
#include "stdio.h"
int main(int argv, char ** argc);

int main(int argv, char ** argc)
{
    char * sStream = (char *) malloc (BIG);
    sprintf(sStream, "GET /index.html HTTP/1.1\nHost: localhost:8080\nUser-Agent: curl/7.54.0\nAccept: */*");
    LOGGER(10,"Initialse Request with %s\n", sStream);
    httpRequest * req = initialiseHttpRequest();
    parseRequest(sStream, req);
    freeHttpRequest(req);
}
