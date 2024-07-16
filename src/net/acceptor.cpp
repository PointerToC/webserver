#include  "net/acceptor.h"
#include  "base/common.h"

#include <unistd.h>


Acceptor::Acceptor(const InetAddress &listen_addr, EventLoop *loop)
  : socket_(CreateSocketFd()),
    loop_(loop),
    channel_(std::make_shared<Channel>(loop, socket_.GetFd())) 
  {
    socket_.SetNoBlock();
    socket_.SetNoDelay();
    socket_.SetReuseAddr();
    socket_.SetReusePort();
    socket_.BindInetAddress(listen_addr);
    channel_->SetReadCallBack(std::bind(&Acceptor::HandleRead, this));
  }

void Acceptor::Listen() {
  socket_.Listen();
  is_listen_ = true;
}

void Acceptor::HandleRead() {
  InetAddress clnt_addr;
  int conn_fd = socket_.Accept(clnt_addr);
  if (NewConnectCallBack) {
    NewConnectCallBack(conn_fd);
  } else {
    close(conn_fd);
  }
}
