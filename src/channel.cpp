#include "channel.h"


const int Channel::NONE_EVEVT = 0;
const int Channel::READ_EVEVT = POLLIN | POLLPRI;
const int Channel::WRITE_EVEVT = POLLOUT;

Channel::Channel(EventLoop* loop, int fd) : 
  loop_(loop), fd_(fd), events_(0), revents_(0), index_(-1) {}

void Channel::HandleEvents() {
  // 远端已经关闭链接，同时没有可读数据
  if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {
    events_ = 0;
    return;
  }

  if (revents_ & EPOLLERR) {
    if (ErrorCallBack) {
      ErrorCallBack();
    }
    return;
  }

  // 可读 | 优先级数据 | 对端半关闭
  if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
    if (ReadCallBack) {
      ReadCallBack();
    }
  }

  // 可写
  if (revents_ & EPOLLOUT) {
    if (WriteCallBack) {
      WriteCallBack();
    }
  }

  // 重置监听状态
  if (ConnectCallBack) {
    ConnectCallBack();
  }
}

void Channel::SetReadCallBack(const EventCallBack& func) {
  ReadCallBack = func;
}

void Channel::SetWriteCallBack(const EventCallBack& func) {
  WriteCallBack = func;
}

void Channel::SetConnectCallBack(const EventCallBack& func) {
  ConnectCallBack = func;
}

void Channel::SetErrorCallBack(const EventCallBack& func) {
  ErrorCallBack = func;
}

int Channel::GetFd() {
  return fd_;
}

int Channel::GetEvents() const {
  return events_;
}

void Channel::SetRevents(int events) {
  revents_ = events;
}

void Channel::SetEvents(int events) {
  events_ = events;
}

bool Channel::IsNoneEvent() {
  
}

void Channel::EnableReading() {

}
