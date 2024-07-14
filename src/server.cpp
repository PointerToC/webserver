#include "server.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


Server::Server(int port, int thread_num, EventLoop *base_loop) : 
  port_(port), thread_num_(thread_num), base_loop_(base_loop) {}

Server::~Server() {}

void Server::Init() {
  // init listen_fd_
  listen_fd_ = CreateSocketFd(port_);
  listen_channel_ = std::make_shared<Channel>(base_loop_, listen_fd_);
  listen_channel_->SetEvents(EPOLLIN | EPOLLET);
  listen_channel_->SetReadCallBack(std::bind(&Server::HandleNewConnect, this));
  listen_channel_->SetConnectCallBack(std::bind(&Server::HandleThisConnect, this));
  base_loop_->EpollAdd(listen_channel_, -1);
  // init thread_pool
  thread_pool_ = std::make_unique<EventLoopThreadPool>(base_loop_, thread_num_);
  // ignore SIGPIPE
  IgnoreSIGPIPE();
  // set listen_fd no block
  SetSocketNoBlock(listen_fd_);
  // 禁用Nagle算法
  SetSocketNoDelay(listen_fd_);
}

void Server::Start() {
  thread_pool_->Start();
  started_ = true;
}

void Server::HandleNewConnect() {
  assert(started_);
  struct sockaddr_in clnt_addr;
  memset(&clnt_addr, 0, sizeof(clnt_addr));
  socklen_t clnt_addr_size = sizeof(clnt_addr);
  int accept_fd = -1;
  while ((accept_fd = accept(listen_fd_, (struct sockaddr *)(&clnt_addr), &clnt_addr_size)) != -1) {
    SetSocketNoBlock(accept_fd);
    SetSocketNoDelay(accept_fd);
    EventLoop *new_loop = thread_pool_->GetNextLoop();
  }
}

void Server::HandleThisConnect() {
  base_loop_->EpollMod(listen_channel_, -1);
}
