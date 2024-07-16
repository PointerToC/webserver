#include "net/inet_address.h"
#include "base/common.h"

#include <cstring>
#include <arpa/inet.h>


InetAddress::InetAddress(int port) {
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = htonl(INADDR_ANY);
  addr_.sin_port = port;
}

InetAddress::InetAddress(const std::string& ip_addr, int port) {
  memset(&addr_, 0, sizeof(addr_));
  int res = inet_pton(AF_INET, ip_addr.c_str(), &addr_.sin_addr);
  if (res == 0) {
    HANDLE_ERROR("Invalid expr");
    abort();
  } else if (res == -1) {
    HANDLE_ERROR("inet_pton failed!");
  }
  addr_.sin_family = AF_INET;
  addr_.sin_port = port;
}

