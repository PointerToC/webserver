#pragma once

#include "count_down_latch.h"
#include "common.h"

#include <functional>


class EventLoop {
  public:
    EventLoop();
    ~EventLoop();
    EventLoop &operator=(const EventLoop &) = delete;
    EventLoop (const EventLoop &) = delete;
    
    // 事件循环
    void Loop();

    // 每一个线程只能拥有一个EventLoop
    void AssertInLoopThread();
    bool IsInLoopThread();

  private:
    bool is_looping_;
    const pid_t thread_id_;
    // 用于唤醒本线程
    int wakeup_fd_;

    void AbortNotInLoopThread();
};
