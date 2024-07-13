#include "channel.h"


const int Channel::NONE_EVEVT = 0;
const int Channel::READ_EVEVT = POLLIN | POLLPRI;
const int Channel::WRITE_EVEVT = POLLOUT;

Channel::Channel(EventLoop* loop, int fd) : loop_(loop), fd_(fd), events_(0), revents_(0), index_(-1) {}

void Channel::HandleEvents() {
  if (revents_ & (POLLIN | POLLPRI )) {

  }

  if (revents_ & POLLIN) {

  }

  if (revents_ & POLLIN) {

  }
}

void Channel::SetReadCallBack(const EventCallBack& func) {
  readCallBack_ = func;
}

void Channel::SetWriteCallBack(const EventCallBack& func) {
  writeCallBack_ = func;
}

void Channel::SetErrorCallBack(const EventCallBack& func) {
  errorCallBack_ = func;
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

bool Channel::IsNoneEvent() {
  
}

void Channel::EnableReading() {

}
