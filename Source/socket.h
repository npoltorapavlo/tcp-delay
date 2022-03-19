#pragma once

#include <stdint.h>
#include <sys/types.h>

class Socket {
public:
    Socket();
    Socket(int new_fd);
    virtual ~Socket();

public:
    auto Create() -> bool;
    auto Connect(uint16_t port) -> bool;
    auto Listen(uint16_t port, int backlog) -> bool;
    auto Accept(bool& eagain) -> int;
    auto Receive(void* buf, size_t len, bool& eagain) -> ssize_t;
    auto Write(const void* buf, size_t len, bool& eagain) -> ssize_t;
    auto Close() -> bool;
    auto SetNoDelay(int flag) -> bool;
    auto SetRcvBuf(uint32_t receiveBuffer) -> bool;
    auto SetSndBuf(uint32_t sendBuffer) -> bool;
    auto SetNonBlocking() -> bool;

protected:
    int _fd;
    uint32_t _receiveBuffer;
    uint32_t _sendBuffer;
};
