#define EPOLL
#define HTTPSERVER_IMPL
#include "httpserver.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int request_target_is(struct http_request_s* request, char const * target) {
  http_string_t url = http_request_target(request);
  int len = strlen(target);
  return len == url.len && memcmp(url.buf, target, url.len) == 0;
}

void handle_request(struct http_request_s* request) {
  char buf[8192];
  http_request_connection(request, HTTP_CLOSE);
  struct http_response_s* response = http_response_init();

  if (request_target_is(request, "/favicon.ico")) {
    http_response_status(response, 204);
  } else {
    http_response_status(response, 200);
    http_response_header(response, "Content-Type", "text/plain");
    int iter = 0, i = 0;
    http_string_t key, val;
    http_string_t url = http_request_target(request);
    i += snprintf(buf + i, 8192 - i, "URL:\n\n");
    i += snprintf(buf + i, 8192 - i, "  %.*s\n\n",url.len,url.buf);
    i += snprintf(buf + i, 8192 - i, "HEADERS:\n\n");
    while (http_request_iterate_headers(request, &key, &val, &iter)) {
      i += snprintf(buf + i, 8192 - i, "  %.*s: %.*s\n", key.len, key.buf, val.len, val.buf);
    }
    http_response_body(response, buf, i);
  }
  http_respond(request, response);
}

struct http_server_s* server;

void handle_sigterm(int signum) {
  free(server);
  exit(0);
}

int main() {
int port;
  port=8000;
  server = http_server_init(port, handle_request);
  signal(SIGTERM, handle_sigterm);
  signal(SIGINT, handle_sigterm);
  http_server_listen(server);
  return 1;
}
