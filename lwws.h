#define LISTEN_PORT 8080
#define LOGLEVEL 10
#define BIG 1024      //Asking for trouble
#define VERYBIG 65635 // asking fro bigger trouble
#define PUBLICLOCATION "./public/"
#define DEFAULTROOTFILE "index.html"

#define LOGGER(l, fmt, ...) if(l<LOGLEVEL){printf("%s: ", getTime()); printf(fmt, __VA_ARGS__);}

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

} httpResponse;

int handleReturn(char * name, int i);
int parseRequest(char * s, httpRequest * req);
struct httpResponse getResource(char * r);
struct httpResponse constructResponse(struct httpRequest req);
const char * getTime();
