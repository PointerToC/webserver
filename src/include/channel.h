#pragma once

#include "event_loop.h"

#include <sys/epoll.h>
#include <functional>
#include <string>


class Channel {
  public:
    typedef std::function<void()> EventCallBack;

    Channel(EventLoop *loop, int fd);
    ~Channel();

    void HandleEvents();
  private:
    EventLoop* loop_;
    const int fd_;

    int events_;
    int revents_;

    EventCallBack readCallBack_;
    EventCallBack writeCallBack_;
    EventCallBack errorCallBack_;
};
