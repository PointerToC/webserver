#pragma once

#include "base/count_down_latch.h"
#include "base/common.h"
#include "net/epoll.h"
#include "net/channel.h"

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
    void Quit();
    // 每一个线程只能拥有一个EventLoop
    void AssertInLoopThread();
    bool IsInLoopThread();

    void EpollAdd(std::shared_ptr<Channel> &req, int timeout);
    void EpollMod(std::shared_ptr<Channel> &req, int timeout);
    void EpollDel(std::shared_ptr<Channel> &req);

    void RunInLoop(std::function<void()> func);
    void QueueInLoop(std::function<void()> func);

  private:
    bool is_looping_;
    bool is_quit_;
    const pid_t thread_id_;
    // 用于唤醒本线程
    int wakeup_fd_;
    std::shared_ptr<Epoll> epoll_;
    std::shared_ptr<Channel> wakeup_channel_{nullptr};
    std::vector<std::function<void()>> PendingFunc;
    MutexLock mtx_;

    void WakeUp();
    void HandleRead();
    void HandleConn();
    void AbortNotInLoopThread();
};
