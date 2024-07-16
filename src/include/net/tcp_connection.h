#pragma once

#include "net/inet_address.h"
#include "net/channel.h"
#include "net/event_loop.h"

class TcpConnection {
  public:
    TcpConnection(const std::string &name, EventLoop *loop, int fd);
    ~TcpConnection();
    TcpConnection(const TcpConnection &) = delete;
    TcpConnection &operator=(const TcpConnection &) = delete;

    void ConnectionEstablished();
  private:
    std::string name_;
    InetAddress local_addr_;
    InetAddress peer_addr_;
    EventLoop *loop_;
    int fd_;
    std::shared_ptr<Channel> channel_;
};
