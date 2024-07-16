#include "net/event_loop_thread.h"


EventLoopThread::EventLoopThread() : cond_(mutex_) {}

EventLoopThread::~EventLoopThread() {}

EventLoop *EventLoopThread::StartLoop() {
  assert(thread_ == nullptr);
  thread_ = std::make_unique<Thread>(std::bind(&EventLoopThread::EventLoopThreadFunc, this), "EventLoopThread");
  thread_->Start();
  MutexLockGuard lck(mutex_);
  while (!loop_) {
    cond_.Wait();
  }
  return loop_.get();
}

void EventLoopThread::EventLoopThreadFunc() {
  {
    MutexLockGuard lck(mutex_);
    loop_ = std::make_unique<EventLoop>();
    loop_->Init();
    cond_.NotifyOne();
  }
  loop_->Loop();
}
