#pragma once

#include "net/inet_address.h"
#include "net/channel.h"
#include "net/event_loop.h"
#include "net/socket.h"
#include "net/buffer.h"


class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
  public:
    TcpConnection(const std::string &name, EventLoop *loop, int fd);
    ~TcpConnection();
    TcpConnection(const TcpConnection &) = delete;
    TcpConnection &operator=(const TcpConnection &) = delete;

    void HandleRead();
    void HandleWrite();
    void HandleClose();
    void HandleError();
    void HandleThisConnect();
    void ConnectionEstablished();
    
    void ReadData(std::string &data);
    void WriteDate(std::string &data);

  private:
    enum TCP_STATE{ CONNECTING, CONNECTED };
    std::string name_;
    InetAddress local_addr_;
    InetAddress peer_addr_;
    EventLoop *loop_;
    Socket socket_;
    std::shared_ptr<Channel> channel_;
    std::unique_ptr<Buffer> input_buffer_;
    std::unique_ptr<Buffer> output_buffer_;
    TCP_STATE state_;

    void SetStatus(TCP_STATE state) { state_ = state; }
};
