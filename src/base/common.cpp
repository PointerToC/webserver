#include "./base/common.h"

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/eventfd.h>
#include <signal.h>
#include <sys/types.h>


int CreateSocketFd() {
  int fd = 0;
  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    HANDLE_ERROR("Create listen_fd failed!");
    return -1;
  }
  return fd;
}

int CreateWakeUpFd() {
  int event_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (event_fd == -1) {
    HANDLE_ERROR("Create eventfd falied");
    abort();
  }
  return event_fd;
}

void IgnoreSIGPIPE() {
  struct sigaction sa;
  sa.sa_handler = SIG_IGN;
  sa.sa_flags = 0;
  if (sigaction(SIGPIPE, &sa, NULL) != 0) {
    HANDLE_ERROR("Ignore SIGPIPE failed");
    abort();
  }
}

