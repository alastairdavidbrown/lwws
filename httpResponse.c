#include "httpResponse.h"
#include "httpRequest.h"
#include "lwws.h"

#include <stdio.h>
#include <stdlib.h>

httpResponse * initialiseHttpResponse()
{
    httpResponse * res = malloc (sizeof(httpResponse));;

    res->headerLength = 0;
    res->contentType = NULL;
    res->contentLength = 0;
    res->responseHeader = NULL;
    res->responseBody = NULL;
    res->response = NULL;
    res->status = NULL;
    return (res);
}

void dumpHttpResponse(httpResponse * res)
{
    LOGGER(10, "Dumping httpResponse: %p\n", res);
    LOGGER(10, "\tres->headerLength: %ld\n", res->headerLength);

    LOGGER(10, "\tres->contentType: %p\n", res->contentType);
    LOGGER(10, "\t\t...value: %s\n", res->contentType);

    LOGGER(10, "\tres->contentLength: %ld\n", res->contentLength);

    LOGGER(10, "\tres->responseHeader: %p\n", res->responseHeader);
    LOGGER(10, "\t\t...value: %s\n", res->responseHeader);

    LOGGER(10, "\tres->responseBody: %p\n", res->responseBody);
    LOGGER(10, "\t\t...value: %s\n", res->responseBody);

    LOGGER(10, "\tres->response: %p\n", res->response);
    LOGGER(10, "\t\t...value: %s\n", res->response);

    LOGGER(10, "\tres->status: %p\n", res->status);
    LOGGER(10, "\t\t...value: %s\n", res->status);

}

void freeHttpResponse(httpResponse * res)
{
    if (res->status)free(res->status);
    if (res->contentType)free(res->contentType);
    //if (res->responseHeader)free(res->responseHeader);
    if (res->responseBody)free(res->responseBody);
    if (res->response)free(res->response);
    if (res) free(res);
    return;
}
