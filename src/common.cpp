#include "common.h"

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/eventfd.h>

inline void PrintError(const std::string &error_str) {
  std::cerr << error_str << std::endl;
}

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

  // SO_REUSEADDR 选项允许一个 socket 绑定到一个已经在使用的地址和端口上。
  // 通常在服务器重启时，如果之前使用的地址和端口尚未完全释放，
  // 设置这个选项可以避免“地址已在使用”错误。
  int optval = 1;
  if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
    close(listen_fd);
    HANDLE_ERROR("Set listen_fd option failed!");
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
