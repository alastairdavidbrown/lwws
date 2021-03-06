#include <stdio.h>
#include <string.h>

#include "httpRequest.h"
#include "httpResponse.h"

#include "lwws.h"
#include "getType.h"

#define EXTENSION 0
#define TYPE 1
#define DEFAULT "default"

char * sTypes[][2] = {
    {"html","text/html"} ,
    {"csv","text/csv"} ,
    {"png","image/png"},
    {"jpg","image/jpg"},
    {"svg","image/svg+xml"},
    {DEFAULT,"text/html"} ,
};

char * getType(char * sExtension)
{
  // LOGGER(10, "getType stubbed, returing %s\n", "text/html");
  // return ("text/html");

  LOGGER(10, "0,0 %s 0,1 %s 1,0 %s, 1,1 %s\n", sTypes[0][0], sTypes[0][1], sTypes[1][0], sTypes[1][1]);
  LOGGER(10, "...mapping extension %s to type\n", (char *) sExtension);

  int i=0;
  while (strcmp(sTypes[i][EXTENSION], sExtension) && strcmp(sTypes[i][EXTENSION], DEFAULT))
      i++;

  LOGGER(10, "...found a mapping for %s at index %d, value is %s\n", sTypes[i][EXTENSION], i, sTypes[i][TYPE]);
  return(sTypes[i][TYPE]);

}
