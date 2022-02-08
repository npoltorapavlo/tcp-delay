#pragma once

#include "socket.h"

class TestSocket : public Socket {
public:
  TestSocket();

  TestSocket(int new_fd);

  auto Receive(const size_t len) -> bool;

  auto Send(const size_t len) -> bool;

  auto SetAppRcvBuf(uint32_t receiveBuffer) -> void;

  auto SetAppSndBuf(uint32_t sendBuffer) -> void;

protected:
  uint32_t _appReceiveBuffer;
  uint32_t _appSendBuffer;
};
