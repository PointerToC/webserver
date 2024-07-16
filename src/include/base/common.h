#pragma once

#include <sys/socket.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <cerrno>
#include <cstring>

// 定义一个宏，用于输出错误信息并终止程序
#define HANDLE_ERROR(msg) \
        std::cerr << "Error: " << msg; \
        std::cerr << " " << "==> File: " << __FILE__; \
        std::cerr << "Line: " << __LINE__ << std::endl; \

// 创建listened socket fd, 绑定到制定端口并且开始监听
int CreateSocketFd();

// 创建event_fd
int CreateWakeUpFd();

// 在服务器程序中，如果客户端意外断开连接，服务器向客户端写数据会导致SIGPIPE信号
// 默认情况下会终止服务器的进程，通过忽略SIGPIPE信号，服务器可以通过其他的方式来处理这种情况
void IgnoreSIGPIPE();
