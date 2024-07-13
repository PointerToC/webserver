#include "event_loop.h"
#include "current_thread.h"

__thread EventLoop *loop_in_thread = 0;


EventLoop::EventLoop() : 
  is_looping_(false),
  thread_id_(CurrentThread::Tid()),
  wakeup_fd_(CreateWakeUpFd())
  {
    if (loop_in_thread == 0) {
      AbortNotInLoopThread();
    } else {
      loop_in_thread = this;
    }
  }

EventLoop::~EventLoop() {
  assert(!is_looping_);
  loop_in_thread = NULL;
}


void EventLoop::Loop() {
  assert(is_looping_ == false);
  AssertInLoopThread();
  is_looping_ = true;

}

void EventLoop::AssertInLoopThread() {
  assert(IsInLoopThread());
}

bool EventLoop::IsInLoopThread() {
  return thread_id_ == CurrentThread::Tid();
}
