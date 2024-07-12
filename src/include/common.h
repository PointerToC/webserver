#pragma once

#include <sys/socket.h>
#include <string>
#include <cstdlib>

// 定义一个宏，用于输出错误信息并终止程序
#define HANDLE_ERROR(msg) \
        std::cerr << "Error: " << msg; \
        std::cerr << " ==> File: " << __FILE__; \
        std::cerr << "Line: " << __LINE__ << std::endl; \

// create listened socket fd
int CreateSocketFd(int port);


