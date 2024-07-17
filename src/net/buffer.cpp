#include "net/buffer.h"


Buffer::Buffer()
  : memo_(DEFAULT_BUFFER_SIZE),
    write_idx_(0),
    read_idx_(0) 
    { }

std::string Buffer::ReadData(size_t read_num) {
  std::string res;
  while (read_idx_ < write_idx_ && read_num != 0) {
    res.push_back(memo_[read_idx_]);
    ++read_idx_;
    --read_num;
  }
  if (read_idx_ == write_idx_) {
    read_idx_ = 0;
    write_idx_ = 0;
  }
  return res;
}

void Buffer::WriteData(std::string &data) {
  memo_.insert(memo_.begin() + write_idx_, data.begin(), data.end());
  write_idx_ += data.size();
}
