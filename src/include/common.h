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
int CreateSocketFd(int port);

// 创建event_fd
int CreateWakeUpFd();
