#include <stdio.h>
#include <string.h>

#include "lwws.h"
#include "getType.h"

#define NUM_KNOWN_TYPES 5
#define EXTENSION 0
#define TYPE 1
#define DEFAULT "default"

const char * sTypes[][2] = {
    {"html","text/html"} ,
    {"csv","text/csv"} ,
    {"png","image/png"},
    {"jpg","image/jpg"},
    {"svg","image/svg+xml"},
    {DEFAULT,"text/html"} ,
};

const char * getType(char * sExtension)
{
  LOGGER(10, "0,0 %s 0,1 %s 1,0 %s, 1,1 %s\n", sTypes[0][0], sTypes[0][1], sTypes[1][0], sTypes[1][1]);
  LOGGER(10, "...mapping extension %s to type\n", (char *) sExtension);

  for(int i=0; i<NUM_KNOWN_TYPES; i++)
  {
    if (!strcmp(sTypes[i][EXTENSION], sExtension))
    {
      LOGGER(10, "...found a mapping for %s at index %d\n", sTypes[i][EXTENSION], i);
      return(sTypes[i][TYPE]);
    }
  }
  return ("text/html"); //default
}
