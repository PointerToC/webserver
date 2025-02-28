#include "net/event_loop.h"
#include "base/current_thread.h"
#include "base/common.h"


__thread EventLoop *loop_in_thread = NULL;

EventLoop::EventLoop()
  : is_looping_(false),
    is_quit_(false),
    thread_id_(CurrentThread::Tid())
    {
      if (loop_in_thread == NULL) {
        loop_in_thread = this;
      } else {
        AbortNotInLoopThread();
      }
    }

EventLoop::~EventLoop() {
  assert(!is_looping_);
  loop_in_thread = NULL;
}

void EventLoop::Init() {
  epoll_ = std::make_shared<Epoll>();
  epoll_->Init();
  wakeup_fd_ = CreateWakeUpFd();
  wakeup_channel_ = std::make_shared<Channel>(this, wakeup_fd_);
  wakeup_channel_->SetEvents(EPOLLIN | EPOLLET);
  wakeup_channel_->SetReadCallBack(std::bind(&EventLoop::HandleRead, this));
  wakeup_channel_->SetConnectCallBack(std::bind(&EventLoop::HandleConn, this));
  EpollAdd(wakeup_channel_, -1);
}

void EventLoop::Loop() {
  assert(!is_looping_);
  AssertInLoopThread();
  is_looping_ = true;
  while (!is_quit_) {
    std::vector<std::shared_ptr<Channel>> res = epoll_->Poll();
    for (auto &channel : res) {
      channel->HandleEvents();
    }
  }
  is_looping_ = false;
}

void EventLoop::Quit() {
  is_quit_ = true;
}

void EventLoop::AssertInLoopThread() {
  assert(IsInLoopThread());
}

bool EventLoop::IsInLoopThread() {
  return thread_id_ == CurrentThread::Tid();
}

void EventLoop::EpollAdd(std::shared_ptr<Channel> &req, int timeout) {
  epoll_->EpollAdd(req, timeout);
}

void EventLoop::EpollMod(std::shared_ptr<Channel> &req, int timeout) {
  epoll_->EpollMod(req, timeout);
}

void EventLoop::EpollDel(std::shared_ptr<Channel> &req) {
  epoll_->EpollDel(req);
}

void EventLoop::QueueInLoop(std::function<void()> func) {
  {
    MutexLockGuard lck(mtx_);
    PendingFunc.emplace_back(std::move(func));
  }

  if (!IsInLoopThread()) {
    WakeUp();
  }
}

void EventLoop::RunInLoop(std::function<void()> func) {
  if (IsInLoopThread()) {
    func();
  } else {
    QueueInLoop(std::move(func));
  }
}

void EventLoop::WakeUp() {
  uint64_t num = 1;
  if (write(wakeup_fd_, &num, sizeof(uint64_t)) != sizeof(uint64_t)) {
    HANDLE_ERROR("Wirte num to wakeup_fd_ failed in WakeUp()");
    abort();
  }
}

void EventLoop::HandleRead() {
  uint64_t num = 0;
  if (read(wakeup_fd_, &num, sizeof(num)) != sizeof(num)) {
    HANDLE_ERROR("Read data from wakeup_fd_ failed in HandleRead()!");
    abort();
  }
}

void EventLoop::HandleConn() {
  wakeup_channel_->SetEvents(EPOLLIN | EPOLLET);
  EpollMod(wakeup_channel_, -1);
}
