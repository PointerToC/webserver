#include "net/buffer.h"

#include <assert.h>

Buffer::Buffer()
  : memo_(DEFAULT_BUFFER_SIZE),
    write_idx_(0),
    read_idx_(0) 
    { }

void Buffer::AddData(int n) {
  assert(write_idx_ + n <= memo_.size());
  write_idx_ += n;
}

void Buffer::RemoveData(int n) {
  assert(read_idx_ + n <= write_idx_);
  read_idx_ += write_idx_;
  if (read_idx_ == write_idx_) {
    read_idx_ = 0;
    write_idx_ = 0;
  }
}

void Buffer::AddSpaceIfFull() {
  int new_sz = memo_.size() * 2;
  if (IsFull()) {
    memo_.resize(new_sz);
  }
}
