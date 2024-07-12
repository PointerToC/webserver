#pragma once

#include <stdint.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>

namespace CurrentThread {
  extern __thread int t_cached_tid_;
  extern __thread char t_tid_string_[32];
  extern __thread int tid_string_length_;
  extern __thread const char* t_thread_name_;

  void CacheTid();

  inline int tid();
}
