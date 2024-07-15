#pragma once

#include <string>
#include <netinet/in.h>

class InetAddress {
  public:
    InetAddress(int port);
    InetAddress(const std::string& ip_addr, int port);
    explicit InetAddress(const struct sockaddr_in &sock_addr) : addr_(sock_addr) {};
  private:
    struct sockaddr_in addr_;
};
