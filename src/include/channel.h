#pragma once

#include "event_loop.h"

#include <sys/epoll.h>
#include <functional>
#include <string>
#include <poll.h>


class Channel {
  public:
    typedef std::function<void()> EventCallBack;

    Channel(EventLoop *loop, int fd);
    ~Channel();

    void HandleEvents();
    void SetReadCallBack(const EventCallBack& func);
    void SetWriteCallBack(const EventCallBack& func);
    void SetErrorCallBack(const EventCallBack& func);
    int GetFd();
    int GetEvents() const;
    void SetRevents(int events);
    bool IsNoneEvent();
    void EnableReading();
    
  private:
    static const int NONE_EVEVT;
    static const int READ_EVEVT;
    static const int WRITE_EVEVT;

    void Update();
    // 一个Channel只对应一个EventLoop
    EventLoop* loop_;
    // 一个Channel只对应一个文件描述符
    const int fd_;
    // 关心的IO事件
    int events_;
    // 目前活动事件
    int revents_;
    int index_;

    EventCallBack readCallBack_;
    EventCallBack writeCallBack_;
    EventCallBack errorCallBack_;
};
