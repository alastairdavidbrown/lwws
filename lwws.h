#define LISTEN_PORT 8080
#define LOGLEVEL 11
#define BIG 1024      //Asking for trouble
#define VERYBIG 65635 // asking fro bigger trouble
#define PUBLICLOCATION "./public/"
#define DEFAULTROOTFILE "index.html"

#define RESP200STATUS "200 OK"
#define RESP404STATUS "404 Not Found"
#define RESP404MESSAGE "404: Not Found, it\'s not there man!"

#define LOGGER(l, fmt, ...) if(l<LOGLEVEL){printf("%s:%s:%d:", getTime(), __FUNCTION__, __LINE__); printf(fmt, __VA_ARGS__);}
//#define LOGGER(l, fmt, ...) 1==1;


struct header{
  char * name;
  char * value;
};

typedef struct httpRequest
{
  long requestId;
  char * method;
  char * resource;
  char * httpVersion;
  int numHeaders;
  struct header * headers;
}httpRequest;

typedef struct httpResponse
{
  long responseId;
  char * contentType;
  long contentLength;
  char * responseHeader;
  char * responseBody;
  char * response;
  char * status;

} httpResponse;

int handleReturn(char * name, int i);
int getResource(char * resourceName, httpResponse * res);
int constructResponse(struct httpRequest req, httpResponse *  res);
int initialiseReqRes(httpRequest * req, httpResponse * res);
int freeReqRes(httpRequest * req, httpResponse * res);
const char * getTime();
