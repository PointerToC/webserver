#include "net/tcp_connection.h"

TcpConnection::TcpConnection(const std::string &name, EventLoop *loop, int fd)
  : name_(name),
    loop_(loop),
    fd_(fd),
    channel_(std::make_shared<Channel>(loop_, fd_))
    { }

void TcpConnection::ConnectionEstablished() {
  loop_->AssertInLoopThread();
  channel_->EnableReading();
}
