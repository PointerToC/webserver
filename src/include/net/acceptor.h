#pragma once

#include "net/channel.h"
#include "net/event_loop.h"
#include "net/socket.h"

class Acceptor {
  public:
    Acceptor(const InetAddress &listen_addr, EventLoop *loop);
    ~Acceptor();
    Acceptor(const Acceptor &) = delete;
    Acceptor &operator=(const Acceptor &) = delete;
    void HandleRead();

    void Listen();
    bool IsListen() { return is_listen_; }
    void SetNewConnectCallBack(std::function<void(int)> func) { NewConnectCallBack = func; }

  private:
    bool is_listen_{false};

    Socket socket_;
    EventLoop *loop_;
    std::shared_ptr<Channel> channel_;

    std::function<void(int)> NewConnectCallBack;
};
