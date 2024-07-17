#include "net/tcp_connection.h"

TcpConnection::TcpConnection(const std::string &name, EventLoop *loop, int fd)
  : name_(name),
    loop_(loop),
    socket_(fd),
    channel_(std::make_shared<Channel>(loop_, socket_.GetFd())),
    state_(CONNECTING) { 
      socket_.SetNoBlock();
      channel_->SetReadCallBack(std::bind(&TcpConnection::HandleRead, shared_from_this()));
      channel_->SetWriteCallBack(std::bind(&TcpConnection::HandleWrite, shared_from_this()));
      channel_->SetCloseCallBack(std::bind(&TcpConnection::HandleClose, shared_from_this()));
      channel_->SetErrorCallBack(std::bind(&TcpConnection::HandleError, shared_from_this()));
      channel_->SetConnectCallBack(std::bind(&TcpConnection::HandleThisConnect, shared_from_this()));
    }

void TcpConnection::HandleRead() {

}

void TcpConnection::HandleWrite() {

}

void TcpConnection::HandleClose() {

}

void TcpConnection::HandleError() {

}

void TcpConnection::HandleThisConnect() { 
  channel_->SetEvents(EPOLLIN | EPOLLET); 
  loop_->EpollMod(channel_, -1);
}

void TcpConnection::ConnectionEstablished() {
  loop_->AssertInLoopThread();
  assert(state_ == CONNECTING);
  SetStatus(CONNECTED);
  channel_->EnableReading();
}

