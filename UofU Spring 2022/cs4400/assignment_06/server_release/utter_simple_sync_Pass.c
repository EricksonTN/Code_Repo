/*
 * utter.c - [Starting code for] a web-based Ute social network.
 *
 * Based on:
 *  tiny.c - A simple, iterative HTTP/1.0 Web server that uses the
 *      GET method to serve static and dynamic content.
 *   Tiny Web server
 *   Dave O'Hallaron
 *   Carnegie Mellon University
 */
#include "csapp.h"
#include "dictionary.h"
#include "more_string.h"

static void doit(int fd);
static dictionary_t *read_requesthdrs(rio_t *rp);
static void read_postquery(rio_t *rp, dictionary_t *headers, dictionary_t *d);
static void clienterror(int fd, char *cause, char *errnum,
                        char *shortmsg, char *longmsg);
static void print_stringdictionary(dictionary_t *d);

static void serve_request(int fd, dictionary_t *query);


static void serve_listen (int fd, char *user);
static void serve_utter (int fd, char *user, char* message);
static void serve_shh (int fd, char *user, char *msgID);
static void serve_sync(int fd, char *hostname, char* port, char *user);
static dictionary_t*  userDictionary_get(char *userID);

//Global Variables
static dictionary_t *users;
static int utterCount = 0;
static char* port;
static char hostname[255];


#ifdef SILENCE_PRINTF
#define printf(...)
#endif


int main(int argc, char **argv) {

  users = make_dictionary(COMPARE_CASE_INSENS, free_dictionary);
  utterCount = 0;
  port = argv[1];
  gethostname(hostname,255);

  
  int listenfd, connfd;
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;

  /* Check command line args */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  listenfd = Open_listenfd(argv[1]);

  /* Don't kill the server if there's an error, because
     we want to survive errors due to a client. But we
     do want to report errors. */
  exit_on_error(0);

  /* Also, don't stop on broken connections: */
  Signal(SIGPIPE, SIG_IGN);

  while (1) {
    clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    if (connfd >= 0) {
      Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE,
          port, MAXLINE, 0);
      printf("Accepted connection from (%s, %s)\n", hostname, port);

      doit(connfd);
      Close(connfd);
      printf("connection closed\n");
    }
  }
}

/*
 * doit - handle one HTTP request/response transaction
 */
void doit(int fd){

  char buf[MAXLINE], *method, *uri, *version;
  rio_t rio;
  dictionary_t *headers, *query;

  /* Read request line and headers */
  Rio_readinitb(&rio, fd);
  if (Rio_readlineb(&rio, buf, MAXLINE) <= 0)
    return;
  printf("%s", buf);

  if (!parse_request_line(buf, &method, &uri, &version)) {
    clienterror(fd, method, "400", "Bad Request",
                "Utter did not recognize the request");
  } else {
    if (strcasecmp(version, "HTTP/1.0")
        && strcasecmp(version, "HTTP/1.1")) {
      clienterror(fd, version, "501", "Not Implemented",
          "Utter does not implement that version");
    } else if (strcasecmp(method, "GET")
        && strcasecmp(method, "POST")){
      clienterror(fd, method, "501", "Not Implemented",
          "Utter does not implement that method");
    } else {
      headers = read_requesthdrs(&rio);

      /* Parse all query arguments into a dictionary */
      query = make_dictionary(COMPARE_CASE_SENS, free);
      parse_uriquery(uri, query);
      if (!strcasecmp(method, "POST")){
        read_postquery(&rio, headers, query);
      }
      /*For debugging, print the dictionary */
      printf("query params\n");
      print_stringdictionary(query);
      printf("URI: %s\n", uri);


      /* You'll want to handle different queries here,
         but the intial implementation always returns
         "hello world": */
      //serve_request(fd, query);

      if (starts_with("/utter", uri))
      {
	serve_utter(fd, dictionary_get(query, "user"), dictionary_get(query, "utterance"));
      }

      else if(starts_with("/listen", uri))
      {
	serve_listen(fd, dictionary_get(query, "user"));
      }

      else if(starts_with("/shh", uri))
      {
	serve_shh(fd, dictionary_get(query, "user"), dictionary_get(query, "id"));
      }

      else if(starts_with("/sync", uri))
      {
	serve_sync(fd, dictionary_get(query, "hostname"), dictionary_get(query, "port"), dictionary_get(query, "user"));
      }
      
      /* Clean up */
      free_dictionary(query);
      free_dictionary(headers);
    }

    /* Clean up status line */
    free(method);
    free(uri);
    free(version);

  }

}

/*
 * read_requesthdrs - read HTTP request headers
 */
dictionary_t *read_requesthdrs(rio_t *rp) {
  char buf[MAXLINE];
  dictionary_t *d = make_dictionary(COMPARE_CASE_INSENS, free);

  Rio_readlineb(rp, buf, MAXLINE);
  //printf("%s", buf);
  while(strcmp(buf, "\r\n")) {
    Rio_readlineb(rp, buf, MAXLINE);
    //printf("%s", buf);
    parse_header_line(buf, d);
  }

  return d;
}

void read_postquery(rio_t *rp, dictionary_t *headers, dictionary_t *dest) {
  char *len_str, *type, *buffer;
  int len;

  len_str = dictionary_get(headers, "Content-Length");
  len = (len_str ? atoi(len_str) : 0);

  type = dictionary_get(headers, "Content-Type");

  buffer = malloc(len+1);
  Rio_readnb(rp, buffer, len);
  buffer[len] = 0;

  if (!strcasecmp(type, "application/x-www-form-urlencoded")) {
    parse_query(buffer, dest);
  }

  free(buffer);
}

static char *ok_header(size_t len, const char *content_type) {
  char *len_str, *header;

  header = append_strings("HTTP/1.0 200 OK\r\n",
                          "Server: Utter Web Server\r\n",
                          "Connection: close\r\n",
                          "Content-length: ", len_str = to_string(len), "\r\n",
                          "Content-type: ", content_type, "\r\n\r\n",
                          NULL);
  free(len_str);

  return header;
}




/*
 * serve_request - example request handler
 */
static void serve_request(int fd, dictionary_t *query) {
  size_t len;
  char *body, *header;

  body = strdup("hello world!");

  len = strlen(body);

  /* Send response headers to client */
  header = ok_header(len, "text/plain; charset=utf-8");
  Rio_writen(fd, header, strlen(header));
  printf("Response headers:\n");
  printf("%s", header);

  free(header);

  /* Send response body to client */
  Rio_writen(fd, body, len);

  free(body);
}


static void serve_listen (int fd, char *user)
{

  printf("in serve_listen\n");
  int i, count;
  size_t len;
  char *header, *returnPrint;

  returnPrint = "";
  
  dictionary_t *userPostDic = userDictionary_get(user);
  count = dictionary_count(userPostDic);

  // NOTE: dictionary_keys mallocs space for the char** array, so make sure to free it after you are done
  //make a for loop for each key value
  for(i = 0; i < count; i++)
  {
    // figure out how to get all the utters together to send to user
    returnPrint = append_strings(returnPrint, dictionary_key(userPostDic, i), " ", (const char *)dictionary_value(userPostDic, i), "\n", NULL);
  }
  
  len = strlen(returnPrint);

  
  /* Send response headers to client */
  header = ok_header(len, "text/plain; charset=utf-8");
  Rio_writen(fd, header, strlen(header));
  printf("Response headers:\n");
  printf("%s", header);

  free(header);

  Rio_writen(fd, returnPrint, len);

  if (returnPrint != "")
  {
    free(returnPrint);
  }
}

static void serve_utter (int fd, char *user, char* message)
{
  size_t len;
  char *header;
  
  //Increment the number of posts
  utterCount++;
  
  //Make a unique Post ID for the message  
  // free the allocated string for utterCount
  char* postNum = to_string(utterCount);
  char* uniqPostID = append_strings(hostname, "_", port, "_", postNum, NULL);
  len = strlen(append_strings(uniqPostID, "\n", NULL));

  free(postNum);
  
  //Get the User's Post Dictionary
  dictionary_t *userPostDic = userDictionary_get(user);

  // set the post dictionary key with the unique post ID & the value as the message
  dictionary_set(userPostDic, uniqPostID, strdup(message));

  //check that the post is now in dictionary(?)
  
  //Print out the uniquePostID

  /* Send response headers to client */
  header = ok_header(len, "text/plain; charset=utf-8");
  Rio_writen(fd, header, strlen(header));
  printf("Response headers:\n");
  printf("%s", header);

  free(header);

  Rio_writen(fd, append_strings(uniqPostID, "\n", NULL), len);

  free(uniqPostID);
  
}

static void serve_shh (int fd, char *user, char *msgID)
{
  char *header;
  size_t len;
  
  dictionary_t *userPostDic = userDictionary_get(user);

  dictionary_remove(userPostDic, msgID);

  len = strlen("\r\n");
  
  header = ok_header(0, "text/plain; charset=utf-8");
  Rio_writen(fd, header, strlen(header));
  printf("Response headers:\n");
  printf("%s", header);

  free(header);
}

static void serve_sync(int fd, char *hostname, char* port, char *user)
{
  printf("inside serve_sync\n");
  int i, count;
  //char * header;
  char buf[MAXLINE];
  rio_t rio;

  size_t n;

  dictionary_t *userPostDic = userDictionary_get(user);
  
  int diffServfd = Open_clientfd(hostname, port);
  char* diffServReq = append_strings("GET /listen?user=", user, " HTTP/1.1", "\r\n\r\n", NULL);
  //char* diffServReq = append_strings(hostname,":", port, "/listen?user=", user, "\r\n\r\n", NULL);

  printf("%s\n", diffServReq);
  
  Rio_writen(diffServfd, diffServReq, strlen(diffServReq));

  

  Rio_readinitb(&rio, diffServfd);
  
  while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
  {
    char** post = split_string(buf, " ");
    char* key = post[0];
    char* value = post[1];
    dictionary_set(userPostDic, key, value);
  }

  /* header = ok_header(0, "text/plain; charset=utf-8"); */
  /* Rio_writen(fd, header, strlen(header)); */
  /* printf("Response headers:\n"); */
  /* printf("%s", header); */

  /* free(header); */

  free(diffServReq);
  
  close(diffServfd);
  
  // Get the different server user's posts

  
  
}

static dictionary_t*  userDictionary_get(char *userID)
{
  //User ID doesn't exist in users dictionary
  if(dictionary_get(users, userID) == NULL)
  {
    dictionary_set(users, userID, make_dictionary(COMPARE_CASE_INSENS, free));
    return dictionary_get(users, userID);
  }

  //User ID exists in users dictionary
  return dictionary_get(users, userID);
  
}

/*
 * clienterror - returns an error message to the client
 */
void clienterror(int fd, char *cause, char *errnum,
		 char *shortmsg, char *longmsg) {
  size_t len;
  char *header, *body, *len_str;

  body = append_strings("<html><title>Utter Error</title>",
                        "<body bgcolor=""ffffff"">\r\n",
                        errnum, " ", shortmsg,
                        "<p>", longmsg, ": ", cause,
                        "<hr><em>Utter Server</em>\r\n",
                        NULL);
  len = strlen(body);

  /* Print the HTTP response */
  header = append_strings("HTTP/1.0 ", errnum, " ", shortmsg, "\r\n",
                          "Content-type: text/html; charset=utf-8\r\n",
                          "Content-length: ", len_str = to_string(len), "\r\n\r\n",
                          NULL);
  free(len_str);

  Rio_writen(fd, header, strlen(header));
  Rio_writen(fd, body, len);

  free(header);
  free(body);
}

static void print_stringdictionary(dictionary_t *d) {
  int i, count;

  count = dictionary_count(d);
  for (i = 0; i < count; i++) {
    printf("%s=%s\n",
           dictionary_key(d, i),
           (const char *)dictionary_value(d, i));
  }
  printf("\n");
}
