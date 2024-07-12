#pragma once

#include "count_down_latch.h"

#include <pthread.h>
#include <functional>


class Thread {
  public:
    explicit Thread(const std::function<void()> &func, std::string thread_name = "Default name");
    ~Thread();

    void Start();
    int Join();
    bool IsStarted() { return started_; }
    pid_t Tid() { return tid_; }
    const std::string &Name() const { return thread_name_; }

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

  private:
    std::function<void()> thread_func_;
    bool started_;
    bool joined_;
    pthread_t pthread_id_;
    pid_t tid_;
    std::string thread_name_;
    // 用控制该线程函数真正跑起来的时候，才返回
    CountDownLatch thread_latch_;
};
