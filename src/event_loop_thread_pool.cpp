#include "event_loop_thread_pool.h"


EventLoopThreadPool::EventLoopThreadPool(EventLoop *base_loop, int thread_num) : 
   base_loop_(base_loop), thread_num_(thread_num), started_(false), next_(0) {}

EventLoopThreadPool::~EventLoopThreadPool() {

}

void EventLoopThreadPool::Start() {
  if (thread_num_ <= 0) {
    HANDLE_ERROR("thread_num_ < 0");
    abort();
  }
  base_loop_->AssertInLoopThread();
  for (int i = 0; i < thread_num_; ++i) {
    std::unique_ptr<EventLoopThread> thread = std::make_unique<EventLoopThread>();
    EventLoop *loop = thread->StartLoop();
    assert(loop != nullptr);
    loops_.emplace_back(loop);
    loop_threads_.emplace_back(std::move(thread));
  }
  started_ = true;
}

EventLoop* EventLoopThreadPool::GetNextLoop() {
  base_loop_->AssertInLoopThread();
  assert(started_);
  // 处理线程池只有一个base_loop的情况
  EventLoop *res = base_loop_;
  if (!loops_.empty()) {
    res = loops_[next_];
    next_ = (next_ + 1) % thread_num_;
  }
  return res;
}
