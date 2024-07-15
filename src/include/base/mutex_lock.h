#pragma once

#include <pthread.h>

class MutexLock {
  public:
    MutexLock() {
      pthread_mutex_init(&mtx_, NULL);
    }

    ~MutexLock() {
      pthread_mutex_lock(&mtx_);
      pthread_mutex_destroy(&mtx_);
    }

    MutexLock &operator=(const MutexLock&) = delete;
    MutexLock(const MutexLock&) = delete;

    void Lock() {
      pthread_mutex_lock(&mtx_);
    }

    void UnLock() {
      pthread_mutex_unlock(&mtx_);
    }

    pthread_mutex_t *GetPtr() {
      return &mtx_;
    }

  private:
    pthread_mutex_t mtx_;
};

class MutexLockGuard {
  public:
    explicit MutexLockGuard(MutexLock &mutex_lock) : mutex_lock_(mutex_lock) { mutex_lock_.Lock(); };

    ~MutexLockGuard() { mutex_lock_.UnLock(); };

    MutexLockGuard &operator=(const MutexLockGuard&) = delete;

    MutexLockGuard(const MutexLockGuard&) = delete;

  private:
    MutexLock &mutex_lock_;
};
