#include "epoll.h"
#include "common.h"

#include <unistd.h>
#include <sys/epoll.h>


Epoll::~Epoll() {
  close(epoll_fd_);
}

void Epoll::Init() {
  if ((epoll_fd_ = epoll_create1(EPOLL_CLOEXEC) == -1)) {
    HANDLE_ERROR("Create epoll instance failed!");
    abort();
  }
  events_.resize(EPOLL_SIZE);
}

void Epoll::EpollAdd(std::shared_ptr<Channel> &req, int timeout) {
  struct epoll_event ev;
  int fd = req->GetFd();
  ev.events = req->GetEvents();
  ev.data.fd = fd;
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
    HANDLE_ERROR("Epoll Add failed");
    abort();
  } else {
    fd_to_channel_.emplace(fd, req);
  }
}

void Epoll::EpollMod(std::shared_ptr<Channel> &req, int timeout) {
  struct epoll_event ev;
  int fd = req->GetFd();
  ev.events = req->GetEvents();
  ev.data.fd = fd;
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &ev) == -1) {
    HANDLE_ERROR("Epoll mod failed");
    abort();
  }
}

void Epoll::EpollDel(std::shared_ptr<Channel> &req) {

}

std::vector<std::shared_ptr<Channel>> Epoll::Poll() {
  while (true) {
    int events_cnt = epoll_wait(epoll_fd_, &(*events_.begin()), EPOLL_SIZE, -1);
    if (events_cnt < 0) {
      HANDLE_ERROR("Epoll wait failed");
      abort();
    } else {
      std::vector<std::shared_ptr<Channel>> res = GetActiveEvents(events_cnt);
      if (!res.empty()) {
        return res;
      }
    }
  }
}

std::vector<std::shared_ptr<Channel>> Epoll::GetActiveEvents(int events_cnt) {
  std::vector<std::shared_ptr<Channel>> res;
  for (int i = 0; i < events_cnt; ++i) {
    struct epoll_event ep_event  = events_[i];
    int fd = ep_event.data.fd;
    int events = ep_event.events;
    auto &channel = fd_to_channel_.at(fd);
    channel->SetRevents(events);
    res.emplace_back(channel);
  }
  return res;
}
