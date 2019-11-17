#include <stdio.h>

#include "lwws.h"
#include "reportMemory.h"

void reportMemory( httpResponse * res)
{
  LOGGER(10,"res->response:%p\n", (res->response));
  LOGGER(10,"res->responseBody:%p\n", &(res->responseBody));
  LOGGER(10,"res->status:%p\n", &(res->status));
}
