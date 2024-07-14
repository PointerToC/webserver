#pragma once

#include "mutex_lock.h"
#include <pthread.h>

class ConditionVariable {
  public:
    ConditionVariable(MutexLock &mutex_lock) : mutex_lock_(mutex_lock) {
      pthread_cond_init(&condition_, NULL);
    };

    ~ConditionVariable() { 
      pthread_cond_destroy(&condition_); 
    };

    ConditionVariable &operator=(const ConditionVariable &) = delete;
    ConditionVariable(const ConditionVariable &) = delete;

    void Wait() {
      pthread_cond_wait(&condition_, mutex_lock_.GetPtr());
    };

    void NotifyOne() {
      pthread_cond_signal(&condition_);
    }

    void NotifyAll() {
      pthread_cond_broadcast(&condition_);
    }

  private:
    pthread_cond_t condition_;
    MutexLock &mutex_lock_;
};
