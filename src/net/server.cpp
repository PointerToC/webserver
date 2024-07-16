#include "net/server.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory>
#include <functional>

Server::Server(const InetAddress &listen_addr, int thread_num, EventLoop *loop)
  : loop_(loop),
    acceptor_(std::make_unique<Acceptor>(listen_addr, loop)),
    thread_pool_(std::make_unique<EventLoopThreadPool>(loop_, thread_num)),
    conn_id_(0)
    {
      IgnoreSIGPIPE();
      acceptor_->SetNewConnectCallBack(std::bind(&Server::HandleNewConnect, this, std::placeholders::_1));
    }

Server::~Server() {}

void Server::Start() {
  thread_pool_->Start();
  assert(!acceptor_->IsListen());
  loop_->RunInLoop(std::bind(&Acceptor::Listen, acceptor_.get()));
  started_ = true;
}

void Server::HandleNewConnect(int conn_fd) {
  loop_->AssertInLoopThread();
  EventLoop *io_loop = thread_pool_->GetNextLoop();
  std::string name = "Connection-" + std::to_string(conn_id_);
  ++conn_id_;
  std::shared_ptr<TcpConnection> new_conn = std::make_shared<TcpConnection>(name, io_loop, conn_fd);
  connections_.emplace(name, new_conn);
  io_loop->RunInLoop(std::bind(&TcpConnection::ConnectionEstablished, new_conn.get()));
}

void Server::HandleThisConnect() {

}
