typedef struct httpResponse
{
  long headerLength;
  char * contentType;
  long contentLength;
  char * responseHeader;
  char * responseBody;
  char * response;
  char * status;
} httpResponse;

httpResponse * initialiseHttpResponse();
void dumpHttpResponse(httpResponse * res);
void freeHttpResponse(httpResponse * res);
