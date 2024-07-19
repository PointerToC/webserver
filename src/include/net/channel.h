#pragma once

#include <sys/epoll.h>
#include <functional>
#include <string>
#include <poll.h>
#include <memory>


class EventLoop;

class Channel : public std::enable_shared_from_this<Channel> {
  public:
    typedef std::function<void()> EventCallBack;

    explicit Channel(EventLoop *loop, int fd);
    ~Channel();
    Channel (const Channel&) = delete;
    Channel &operator=(const Channel&) = delete;

    void HandleEvents();
    int GetFd() { return fd_; }
    int GetEvents() const { return events_; }
    void SetRevents(int events) { revents_ = events; }
    void SetEvents(int events) { events_ = events; }
    void UpdateEvents();

    void SetReadCallBack(const EventCallBack& func) { ReadCallBack = func; }
    void SetWriteCallBack(const EventCallBack& func) { WriteCallBack = func; }
    void SetCloseCallBack(const EventCallBack& func) { CloseCallBack = func; }
    void SetErrorCallBack(const EventCallBack& func) { ErrorCallBack = func; }
    void SetConnectCallBack(const EventCallBack& func) { ConnectCallBack = func; }

    void EnableReading();
    void EnableWriting();
    void AddNewEvents(int new_events) { new_events_ |= new_events; };
    
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
    // 通过epoll->Poll()收集到的活动事件
    int revents_;
    int new_events_;
    int index_;

    EventCallBack ReadCallBack;
    EventCallBack WriteCallBack;
    EventCallBack CloseCallBack;
    EventCallBack ConnectCallBack;
    EventCallBack ErrorCallBack;
};
