#pragma once

#include "net/inet_address.h"
#include "net/buffer.h"

const int SOCKET_MAX_LISTEN_QUEUE = 128;

class Socket {
  public:
    Socket(const int fd) : fd_(fd) {}
    ~Socket();

    // 设置listen_fd为非阻塞模式
    void SetNoBlock();
    // 禁用nagle算法
    void SetNoDelay();
    void SetReusePort();
    void SetReuseAddr();
    void BindInetAddress(const InetAddress &inet_addr);
    int GetFd() const { return fd_; }
    void Listen();
    int Accept(InetAddress &clnt_addr);
    int Send(Buffer &buffer, int flag);
    int Receive(Buffer &buffer, int flag);

  private:
    const int fd_;
};
