#include "net/socket.h"
#include "base/common.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

void Socket::SetNoBlock() {
  int flag = fcntl(fd_, F_GETFD, 0);
  if (flag == -1) {
    close(fd_);
    HANDLE_ERROR("Get fd flag failed");
  }
  flag |= O_NONBLOCK;
  if (fcntl(fd_, F_SETFL, flag) == -1) {
    close(fd_);
    HANDLE_ERROR("Set fd flag failed");
  }
}

void Socket::SetNoDelay() {
  int enable = 1;
  if (setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, (void *)(&enable), sizeof(enable)) == -1) {
    close(fd_);
    HANDLE_ERROR("Set fd no delay failed");
  }
}

void Socket::SetReusePort() {
  int optval = 1;
  if (setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof(optval))) == -1) {
    close(fd_);
    HANDLE_ERROR("Set sock addr reuse failed!");
  }
}

void Socket::SetReuseAddr() {
  int optval = 1;
  if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof(optval))) == -1) {
    close(fd_);
    HANDLE_ERROR("Set sock addr reuse failed!");
  }
}

void Socket::BindInetAddress(const InetAddress &inet_addr) {
  const struct sockaddr_in &addr = inet_addr.GetSockAddr();
  if (bind(fd_, (struct sockaddr*)(&addr), sizeof(addr)) == -1) {
    close(fd_);
    HANDLE_ERROR("Bind fd to address failed");
  }
}

void Socket::Listen() {
  if (listen(fd_, SOCKET_MAX_LISTEN_QUEUE) == -1) {
    close(fd_);
    HANDLE_ERROR("Listen listen_fd falied");
  }
}

int Socket::Accept(InetAddress &addr) {
  struct sockaddr_in clnt_addr;
  memset(&clnt_addr, 0, sizeof(clnt_addr));
  socklen_t addr_len = static_cast<socklen_t>(sizeof(clnt_addr));
  int conn_fd = accept4(fd_, (struct sockaddr *)(&clnt_addr), &addr_len, SOCK_NONBLOCK | SOCK_CLOEXEC);
  if (conn_fd < 0) {
    HANDLE_ERROR("Accept failed");
  } else {
    addr.ReSetAddr(clnt_addr);
  }
  return conn_fd;
}

int Socket::Send(Buffer &buffer, int flag) {
  if (buffer.IsEmpty()) {
    return 0;
  }
  int n = send(fd_, buffer.Start(), buffer.DataLen(), flag);
  buffer.RemoveData(n);
  return n;
}

int Socket::Receive(Buffer &buffer, int flag) {
  int res = 1; // -1 : error, 0 : close, 1 : success
  int recv_size = 0;
  buffer.IncreaseSpaceIfFull();
  while ((recv_size = recv(fd_, buffer.Start(), buffer.UsableSpace(), flag)) > 0) {
    buffer.AddData(recv_size);
    buffer.IncreaseSpaceIfFull();
  }
  if (recv_size == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
    res = -1;
  } else if (recv_size == 0) {
    res = 0;
  }
  return res;
}
