#include "./net/socket.h"
#include "./base/common.h"

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

}

void Socket::BindInetAddress(const InetAddress &inet_addr) {

}

void Socket::Listen() {

}

void Socket::Accept(const InetAdress &inet_addr) {

}
