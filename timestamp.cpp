#include "timestamp.h"

#include <sys/time.h>
#include <time.h>

#include <cstdio>

Timestamp &Timestamp::Instance() {
  static Timestamp instance;

  return instance;
}

const char *Timestamp::Value() {
  struct tm *tm_info;
  struct timeval tv;

  gettimeofday(&tv, nullptr);

  tm_info = localtime(&tv.tv_sec);

  strftime(_buf, BufSize, "%Y.%m.%d %H:%M:%S", tm_info);

  sprintf(_buf + DateTimeLen, ".%06ld", (long) tv.tv_usec);

  return (_buf);
}
