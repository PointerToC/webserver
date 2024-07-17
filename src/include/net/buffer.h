#pragma once

#include <vector>
#include <string>


const int DEFAULT_BUFFER_SIZE = 1024;

class Buffer {
  public:
    Buffer();
    ~Buffer();
    Buffer(const Buffer &) = delete;
    Buffer &operator=(const Buffer &) = delete;

    void WriteData(std::string &data);
    std::string ReadData(size_t read_num);
    size_t ReadableSize() { return write_idx_ - read_idx_; }
  private:
    std::vector<char> memo_;
    size_t write_idx_;
    size_t read_idx_;
    
};
