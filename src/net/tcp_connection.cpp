#include "net/tcp_connection.h"

TcpConnection::TcpConnection(const std::string &name, EventLoop *loop, int fd)
  : name_(name),
    loop_(loop),
    socket_(fd),
    channel_(std::make_shared<Channel>(loop_, socket_.GetFd())),
    state_(CONNECTING)
    { 
      socket_.SetNoBlock();
      channel_->SetReadCallBack(std::bind(&TcpConnection::HandleRead, shared_from_this()));
      channel_->SetWriteCallBack(std::bind(&TcpConnection::HandleWrite, shared_from_this()));
      channel_->SetCloseCallBack(std::bind(&TcpConnection::HandleClose, shared_from_this()));
      channel_->SetErrorCallBack(std::bind(&TcpConnection::HandleError, shared_from_this()));
      channel_->SetConnectCallBack(std::bind(&TcpConnection::HandleThisConnect, shared_from_this()));
    }

void TcpConnection::HandleRead() {
  loop_->AssertInLoopThread();
  assert(state_ == CONNECTED);
  int n = socket_.Receive(input_buffer_, 0);
  if (n == -1) {
    HANDLE_ERROR("HandleRead failed");
    abort();
  } else if (n == 0) {
    // handle close
  }
}

void TcpConnection::HandleWrite() {
  loop_->AssertInLoopThread();
  assert(state_ == CONNECTED);
  int n = socket_.Send(output_buffer_, 0);
  if (n == -1) {
    HANDLE_ERROR("Send msg failed");
    abort();
  }
  if (!output_buffer_.IsEmpty()) {
    // 缓冲区中还有待发送数据，需要注册EPOLLOUT事件
    channel_->AddNewEvents(EPOLLOUT);
  }
}

void TcpConnection::HandleClose() {

}

void TcpConnection::HandleError() {
  HANDLE_ERROR("error");
  abort();
}

void TcpConnection::HandleThisConnect() { 
  channel_->UpdateEvents(); 
  loop_->EpollMod(channel_, -1);
}

void TcpConnection::ConnectionEstablished() {
  loop_->AssertInLoopThread();
  assert(state_ == CONNECTING);
  SetStatus(CONNECTED);
  channel_->EnableReading();
}

