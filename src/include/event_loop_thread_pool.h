#pragma once

#include "event_loop_thread.h"
#include "thread.h"

class EventLoopThreadPool {
  public:
    EventLoopThreadPool(EventLoop *, int);
    ~EventLoopThreadPool();
    EventLoopThreadPool(const EventLoopThreadPool &) = delete;
    EventLoopThreadPool &operator=(const EventLoopThreadPool &) = delete;

    void Start();
    EventLoop* GetNextLoop();

  private:
    EventLoop *base_loop_;
    int thread_num_{0};
    bool started_;
    int next_;
    std::vector<EventLoop *> loops_;
    std::vector<std::unique_ptr<EventLoopThread>> loop_threads_;
};
