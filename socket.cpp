#include "socket.h"

#include "timestamp.h"

#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>

#include <fcntl.h>

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <unistd.h>

#define LOG(fmt, ...) printf("%s [%s] " fmt "\n", Timestamp::Instance().Value(), __func__, ##__VA_ARGS__);

Socket::Socket()
    : _fd(-1), _receiveBuffer(0), _sendBuffer(0) {
}

Socket::Socket(int new_fd)
    : _fd(new_fd), _receiveBuffer(0), _sendBuffer(0) {
}

Socket::~Socket() {
  LOG("[%d]", _fd);

  if (_fd != -1) {
    Close();
  }
}

auto Socket::Create() -> bool {
  bool result = false;

  if ((_fd = ::socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    LOG("[%d] err %d (%s)", _fd, errno, strerror(errno));
  } else {
    LOG("[%d]", _fd);

    int opt = 1;
    if (::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
      LOG("[%d] setsockopt err %d (%s)", _fd, errno, strerror(errno));
    }

    result = true;
  }

  return (result);
}

auto Socket::Connect(uint16_t port) -> bool {
  bool result = false;

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  if (::connect(_fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
    LOG("[%d] err %d (%s)", _fd, errno, strerror(errno));
  } else {
    LOG("[%d]", _fd);

    result = true;
  }

  return (result);
}

auto Socket::Listen(uint16_t port, int backlog) -> bool {
  bool result = false;

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  if (::bind(_fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
    LOG("[%d] bind err %d (%s)", _fd, errno, strerror(errno));
  } else if (::listen(_fd, backlog) == -1) {
    LOG("[%d] listen err %d (%s)", _fd, errno, strerror(errno));
  } else {
    LOG("[%d]", _fd);

    result = true;
  }

  return (result);
}

auto Socket::Accept(bool &eagain) -> int {
  struct sockaddr address;
  socklen_t addrlen = sizeof(address);

  int new_fd = ::accept(_fd, &address, (socklen_t * ) & addrlen);

  if (new_fd == -1) {
    if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {

      // there's nothing to accept at the moment

      eagain = true;
    } else {
      LOG("[%d] err %d (%s)", _fd, errno, strerror(errno));
    }
  } else {
    LOG("[%d] %d", _fd, new_fd);
  }

  return (new_fd);
}

auto Socket::Receive(void *buf, size_t len, bool &eagain) -> ssize_t {
  ssize_t result;

  result = ::recv(_fd, buf, len, 0);

  if (result == -1) {
    if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {

      // there's nothing to receive at the moment

      eagain = true;
    } else {
      LOG("[%d] recv err %d (%s)", _fd, errno, strerror(errno));
    }
  } else if (result == 0) {
    LOG("[%d] eof/zero-length", _fd);
  } else {
    LOG("[%d] %ld bytes (buffer %ld)", _fd, result, len);
  }

  return (result);
}

auto Socket::Write(const void *buf, size_t len, bool &eagain) -> ssize_t {
  ssize_t result;

  result = ::send(_fd, buf, len, 0);

  if (result == -1) {
    if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {

      // output buffer is full

      eagain = true;
    } else {
      LOG("[%d] send err %d (%s)", _fd, errno, strerror(errno));
    }
  } else {
    LOG("[%d] %ld bytes (of %ld)", _fd, result, len);
  }

  return (result);
}

auto Socket::Close() -> bool {
  bool result = false;

  if (::close(_fd) == -1) {
    LOG("[%d] err %d (%s)", _fd, errno, strerror(errno));
  } else {
    LOG("[%d]", _fd);

    result = true;
  }

  return (result);
}

auto Socket::SetNoDelay(int flag) -> bool {
  bool result = false;

  if (::setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int)) == -1) {
    LOG("[%d] err %d (%s)", _fd, errno, strerror(errno));
  } else {
    LOG("[%d] %d", _fd, flag);

    result = true;
  }

  return (result);
}

auto Socket::SetRcvBuf(uint32_t receiveBuffer) -> bool {
  bool result = false;

  if (::setsockopt(_fd, SOL_SOCKET, SO_RCVBUF, (const char *) &receiveBuffer, sizeof(receiveBuffer)) == -1) {
    LOG("[%d] err %d (%s)", _fd, errno, strerror(errno));
  } else {
    LOG("[%d] %d", _fd, receiveBuffer);

    _receiveBuffer = receiveBuffer;

    result = true;
  }

  return (result);
}

auto Socket::SetSndBuf(uint32_t sendBuffer) -> bool {
  bool result = false;

  if (::setsockopt(_fd, SOL_SOCKET, SO_SNDBUF, (const char *) &sendBuffer, sizeof(sendBuffer)) == -1) {
    LOG("[%d] err %d (%s)", _fd, errno, strerror(errno));
  } else {
    LOG("[%d] %d", _fd, sendBuffer);

    _sendBuffer = sendBuffer;

    result = true;
  }

  return (result);
}

auto Socket::SetNonBlocking() -> bool {
  bool result = false;

  if (::fcntl(_fd, F_SETFL, fcntl(_fd, F_GETFL, 0) | O_NONBLOCK) != 0) {
    LOG("[%d] err %d (%s)", _fd, errno, strerror(errno));
  } else {
    LOG("[%d]", _fd);

    result = true;
  }

  return (result);
}
