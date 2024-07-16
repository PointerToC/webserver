#pragma once

#include "net/event_loop_thread_pool.h"
#include "net/channel.h"
#include "net/event_loop.h"
#include "net/socket.h"
#include "net/inet_address.h"
#include "net/acceptor.h"
#include "net/tcp_connection.h"

#include <memory>


class Server {
  public:
    typedef std::function<void()> EventCallBack;

    Server(const InetAddress &listen_addr, int thread_num, EventLoop *base_loop);
    ~Server();
    Server(const Server &) = delete;
    Server &operator=(const Server &) = delete;

    void Start();

  private:
    EventLoop *loop_{nullptr};
    std::unique_ptr<Acceptor> acceptor_{nullptr};
    std::unique_ptr<EventLoopThreadPool> thread_pool_{nullptr};
    std::unordered_map<std::string, std::shared_ptr<TcpConnection>> connections_;
    bool started_{false};
    int conn_id_;

    void HandleNewConnect(int conn_fd);
    void HandleThisConnect();

};
