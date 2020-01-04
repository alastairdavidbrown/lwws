struct requestHeader{
  char * name;
  char * value;
};

typedef struct httpRequest
{
  char * method;
  char * resource;
  char * httpVersion;
  long numHeaders;
  struct requestHeader * requestHeaders;
} httpRequest;

httpRequest * initialiseHttpRequest();
void dumpHttpRequest(httpRequest * req);
void freeHttpRequest(httpRequest * req);
