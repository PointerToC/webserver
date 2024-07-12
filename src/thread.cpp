#include "thread.h"
#include "current_thread.h"
#include "count_down_latch.h"

#include <cassert>
#include <iostream>
#include <cstring>
#include <string>
#include <sys/prctl.h>


// CurrentThread : 获取内核线程号
namespace CurrentThread {
  __thread int t_cached_tid_ = 0;
  __thread char t_tid_string_[32];
  __thread int tid_string_length_ = 6;
  __thread const char* t_thread_name = "default";
}

void CurrentThread::CacheTid() {
  if (t_cached_tid_ == 0) {
    t_cached_tid_ = static_cast<pid_t>(syscall(SYS_gettid));
  }
}

inline int CurrentThread::tid() {
  if (__builtin_expect(t_cached_tid_ == 0, 0)) {
    CacheTid();
  }
  return t_cached_tid_;
}


// ThreadData
struct ThreadData {
  std::function<void()> &thread_func_;
  std::string thread_name_;
  pid_t &tid_;
  CountDownLatch &thread_latch_;

  ThreadData(std::function<void()> &thread_func, std::string &thread_name, pid_t &tid, CountDownLatch &thread_latch)
    : thread_func_(thread_func), thread_name_(thread_name), tid_(tid), thread_latch_(thread_latch) {}
  
  void RunInThread() {
    tid_ = CurrentThread::tid();
    thread_latch_.CountDown();
    // 设置当前的线程名字
    prctl(PR_SET_NAME, thread_name_.c_str());
    thread_func_();
  }
};

void *StartThread(void * arg) {
  ThreadData *obj = static_cast<ThreadData *>(arg);
  obj->RunInThread();
  return NULL;
}


// class Thread
Thread::Thread(const std::function<void()> &thread_func, std::string thread_name)
  : thread_func_(thread_func),
    started_(false),
    joined_(false),
    pthread_id_(0),
    tid_(0),
    thread_name_(thread_name),
    thread_latch_(1) {}

void Thread::Start() {
  ThreadData *thread_data = new ThreadData(thread_func_, thread_name_, tid_, thread_latch_);
  assert(!started_);
  started_ = true;
  int err = pthread_create(&pthread_id_, NULL, StartThread, thread_data);
  if (err != 0) {
    started_ = false;
    delete thread_data;
    std::cout << "pthread_created falied! " << strerror(err) << std::endl;
    exit(EXIT_FAILURE);
  } else {
    thread_latch_.Wait();
    delete thread_data;
  }
}

int Thread::Join() {
  assert(started_);
  assert(!joined_);
  joined_ = true;
  return pthread_join(pthread_id_, NULL);
}
