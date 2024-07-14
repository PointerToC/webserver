#include "common.h"

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/eventfd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netinet/tcp.h>


int CreateSocketFd(int port) {
  if (port < 0 || port > 65535) {
    HANDLE_ERROR("Invalid Port");
    return -1;
  }

  int listen_fd = 0;
  if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    HANDLE_ERROR("Create listen_fd failed!");
    return -1;
  }

  int optval = 1;
  if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
    close(listen_fd);
    HANDLE_ERROR("Set sock addr reuse failed!");
    return -1;
  }

  // 绑定到本机地址
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);
  if (bind(listen_fd, (struct sockaddr*)(&serv_addr), sizeof(serv_addr)) == -1) {
    close(listen_fd);
    HANDLE_ERROR("Bind listen_fd failed");
    return -1;
  }

  // 开始监听
  if (listen(listen_fd, 2048) == -1) {
    close(listen_fd);
    HANDLE_ERROR("Listen listen_fd falied");
  }
  return listen_fd;
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

void SetSocketNoBlock(int fd) {
  int flag = fcntl(fd, F_GETFD, 0);
  if (flag == -1) {
    close(fd);
    HANDLE_ERROR("Get fd flag failed");
  }
  flag |= O_NONBLOCK;
  if (fcntl(fd, F_SETFL, flag) == -1) {
    close(fd);
    HANDLE_ERROR("Set fd flag failed");
  }
}

void SetSocketNoDelay(int fd) {
  int enable = 1;
  if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)(&enable), sizeof(enable)) == -1) {
    close(fd);
    HANDLE_ERROR("Set fd no delay failed");
  }
}
