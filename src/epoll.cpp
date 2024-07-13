#include "epoll.h"
#include "common.h"

#include <unistd.h>


Epoll::~Epoll() {
  close(epoll_fd_);
}

void Epoll::Init() {
  if ((epoll_fd_ = epoll_create1(EPOLL_CLOEXEC) == -1)) {
    HANDLE_ERROR("Create epoll instance failed!");
    abort();
  }
}


void Epoll::EpollAdd(std::shared_ptr<Channel> &req, int timeout) {
  struct epoll_event ev;
  int fd = req->GetFd();
  ev.events = req->GetEvents();
  ev.data.fd = fd;
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
    HANDLE_ERROR("Epoll Add failed");
    abort();
  }
}

void Epoll::EpollMod(std::shared_ptr<Channel> &req) {
  struct epoll_event ev;
  int fd = req->GetFd();
  ev.events = req->GetEvents();
  ev.data.fd = fd;
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &ev) == -1) {
    HANDLE_ERROR("Epoll mod failed");
    abort();
  }
}

void Epoll::EpollDel() {

}

std::vector<std::shared_ptr<Channel>> Epoll::Poll() {

}



