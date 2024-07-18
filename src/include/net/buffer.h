#pragma once

#include <vector>
#include <string>


const int DEFAULT_BUFFER_SIZE = 1024;

class Buffer {
  public:
    Buffer();
    ~Buffer() {}
    Buffer(const Buffer &) = delete;
    Buffer &operator=(const Buffer &) = delete;

    void *Start() { return static_cast<void *>(&memo_[read_idx_]); }
    size_t DataLen() { return write_idx_ - read_idx_; }
    bool IsEmpty() { return read_idx_ == write_idx_; }
    bool IsFull() { return write_idx_ == memo_.size(); }
    void AddData(int n);
    void RemoveData(int n);
    void AddSpaceIfFull();
    size_t UsableSpace() { return memo_.size() - write_idx_; }

  private:
    std::vector<char> memo_;
    size_t write_idx_;
    size_t read_idx_;
};
