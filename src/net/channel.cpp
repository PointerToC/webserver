#include "net/channel.h"
#include "net/event_loop.h"


const int Channel::NONE_EVEVT = 0;
const int Channel::READ_EVEVT = POLLIN | POLLPRI;
const int Channel::WRITE_EVEVT = POLLOUT;

Channel::Channel(EventLoop* loop, int fd) : 
  loop_(loop), fd_(fd), events_(0), revents_(0), index_(-1) {}

void Channel::HandleEvents() {
  // 远端已经关闭链接，同时没有可读数据，关闭连接
  if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {
    if (CloseCallBack) {
      CloseCallBack();
    }
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

  // 因为是边缘触发模式，所以需要重置监听状态
  if (ConnectCallBack) {
    ConnectCallBack();
  }
}

void Channel::EnableReading() {
  SetEvents(EPOLLIN | EPOLLET);
  auto temp_ptr = shared_from_this();
  loop_->EpollAdd(temp_ptr, -1);
}
