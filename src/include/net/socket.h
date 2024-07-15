#pragma once

#include "./net/inet_address.h"

class Socket {
  public:
    Socket(const int fd) : fd_(fd) {}
    ~Socket();

    // 设置listen_fd为非阻塞模式
    void SetNoBlock();

    // 禁用nagle算法
    void SetNoDelay();

    void SetReusePort();

    void BindInetAddress(const InetAddress &inet_addr);

    void Listen();

    void Accept(const InetAddress &inet_addr);
  private:
    const int fd_;
};
