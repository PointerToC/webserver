#pragma once

#include "net/channel.h"

#include <sys/epoll.h>
#include <memory>
#include <unordered_map>
#include <vector>

const int EPOLL_SIZE = 50;

class Epoll {
  public:
    Epoll();
    ~Epoll();
    Epoll (const Epoll &) = delete;
    Epoll &operator=(const Epoll &) = delete;

    void Init();
    void EpollAdd(std::shared_ptr<Channel> &req, int timeout);
    void EpollMod(std::shared_ptr<Channel> &req, int timeout);
    void EpollDel(std::shared_ptr<Channel> &req);
    std::vector<std::shared_ptr<Channel>> Poll();
    std::vector<std::shared_ptr<Channel>> GetActiveEvents(int events_cnt);
  private:
    typedef std::unordered_map<int, std::shared_ptr<Channel>> FdToChannel;

    int epoll_fd_;
    std::vector<struct epoll_event> events_;
    FdToChannel fd_to_channel_;

};
