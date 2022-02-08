#pragma once

#include <stdint.h>
#include <sys/types.h>

struct ClientArgs {
  ClientArgs(int argc, char **argv);

  uint16_t port;
  size_t receiveBuffer;
  size_t sendBuffer;
  size_t appReceiveBuffer;
  size_t appSendBuffer;
  size_t expectedReceiveLength;
  size_t expectedSendLength;
  int noDelay;
  int iterations;
};
