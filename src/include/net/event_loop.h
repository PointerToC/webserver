#pragma once

#include "./base/count_down_latch.h"
#include "./base/common.h"
#include "./net/epoll.h"
#include "./net/channel.h"

#include <functional>
#include <memory>


class EventLoop {
  public:
    EventLoop();
    ~EventLoop();
    EventLoop &operator=(const EventLoop &) = delete;
    EventLoop (const EventLoop &) = delete;
    
    void Init();
    // 事件循环
    void Loop();

    // 每一个线程只能拥有一个EventLoop
    void AssertInLoopThread();
    bool IsInLoopThread();

    void EpollAdd(std::shared_ptr<Channel> &req, int timeout);
    void EpollMod(std::shared_ptr<Channel> &req, int timeout);
    void EpollDel(std::shared_ptr<Channel> &req);

  private:
    bool is_looping_;
    const pid_t thread_id_;
    // 用于唤醒本线程
    int wakeup_fd_;
    std::shared_ptr<Epoll> epoll_;
    std::shared_ptr<Channel> wakeup_channel_{nullptr};

    void WakeUp();
    void HandleRead();
    void HandleConn();
    void AbortNotInLoopThread();
};
