#pragma once

#include "base/condition_variable.h"

#include <cassert>

class CountDownLatch {
  public:
    CountDownLatch(int count) : count_(count), mutex_(), cond_(mutex_) {}
    ~CountDownLatch();

    CountDownLatch &operator=(const CountDownLatch &) = delete;
    CountDownLatch (const CountDownLatch &) = delete;

    void CountDown() {
      MutexLockGuard lck(mutex_);
      --count_;
      if (count_ == 0) {
        cond_.NotifyAll();
      }
    }

    void Wait() {
      MutexLockGuard lck(mutex_);
      while (count_ > 0) {
        cond_.Wait();
      }
    }

  private:
    int count_;
    MutexLock mutex_;
    ConditionVariable cond_;
};
