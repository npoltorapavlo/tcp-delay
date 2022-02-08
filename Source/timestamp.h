#pragma once

class Timestamp {
public:
  static Timestamp &Instance();

  const char *Value();

private:
  static const int BufSize = 27;
  static const int DateTimeLen = 19;

  char _buf[BufSize];
};
