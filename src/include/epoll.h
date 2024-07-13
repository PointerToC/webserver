#pragma once

#include "channel.h"
#include <sys/epoll.h>
#include <memory>
#include <unordered_map>

class Epoll {
  public:
    Epoll();
    ~Epoll();
    void Init();
    void EpollAdd(std::shared_ptr<Channel> &req, int timeout);
    void EpollMod(std::shared_ptr<Channel> &red);
    void EpollDel();
    std::vector<std::shared_ptr<Channel>> Poll();
  private:
    typedef std::unordered_map<int, std::shared_ptr<Channel>> FdToChannel;
    int epoll_fd_;
    std::vector<struct epoll_event> events_;
    FdToChannel fd_to_channel_;
};
