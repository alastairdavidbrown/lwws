#define LISTEN_PORT 8080
#define LOGLEVEL 11
#define BIG 1024      //Asking for trouble
#define VERYBIG 65635 // asking fro bigger trouble
#define BUFFER_SIZE 1024 // asking fro bigger trouble

#define PUBLICLOCATION "public"
#define DEFAULTROOTFILE "index.html"

#define RESP200STATUS "200 OK"
#define RESP404STATUS "404 Not Found"
#define RESP404MESSAGE "404: Not Found, it\'s not there man!"

#define PRINT_TIME_FUNCTION_LINE printf("%s:%s:%d:", getTime(), __FUNCTION__, __LINE__)
#define LOGGER(l, fmt, ...) if(l<LOGLEVEL){PRINT_TIME_FUNCTION_LINE; printf(fmt, __VA_ARGS__);}
#define EXIT_IF_NULL(v) \
    PRINT_TIME_FUNCTION_LINE; printf("comparing %p to NULL\n",v); \
    if(!v){PRINT_TIME_FUNCTION_LINE; printf("value is NULL, exiting...\n");exit (-1);}


int lwws();
int handleReturn(char * name, int i);
int getResource(char * resourceName, httpResponse * res);
int constructResponse(httpRequest * req, httpResponse * res);
int spoofResponse(httpRequest * req, httpResponse *  res);
int freeReqRes(httpRequest * req, httpResponse * res);
const char * getTime();
void sig_handler(int signo);
int setResourceType(char * p, httpResponse * res);
