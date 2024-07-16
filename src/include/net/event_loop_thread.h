#pragma once

#include "base/thread.h"
#include "net/event_loop.h"

#include <memory>

class EventLoopThread {
  public:
    EventLoopThread();
    ~EventLoopThread();
    EventLoopThread(const EventLoopThread &) = delete;
    EventLoopThread &operator=(const EventLoopThread &) = delete;

    EventLoop *StartLoop();

  private:
    void EventLoopThreadFunc();
    std::unique_ptr<EventLoop> loop_{nullptr};
    std::unique_ptr<Thread> thread_{nullptr};
    MutexLock mutex_;
    ConditionVariable cond_;
};


