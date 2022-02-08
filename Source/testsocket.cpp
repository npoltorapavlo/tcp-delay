#include "testsocket.h"

#include <algorithm>

TestSocket::TestSocket()
    : Socket(), _appReceiveBuffer(0), _appSendBuffer(0) {
}

TestSocket::TestSocket(int new_fd)
    : Socket(new_fd), _appReceiveBuffer(0), _appSendBuffer(0) {
}

auto TestSocket::Receive(const size_t len) -> bool {
  if (len == 0) {

    // don't expect anything to receive

    return true;
  }

  const size_t bufSize = (_appReceiveBuffer == 0 ? len : _appReceiveBuffer);

  char buf[bufSize];

  size_t totalRead = 0;
  ssize_t read;

  bool eagain;

  do {
    read = Socket::Receive(buf, bufSize, eagain);

    if (read > 0) {
      totalRead += read;
    } else if ((read == 0) || ((read == -1) && !eagain)) {
      break;
    }
  } while (totalRead < len);

  return (totalRead == len);
}

auto TestSocket::Send(const size_t len) -> bool {
  if (len == 0) {

    // don't expect anything to send

    return true;
  }

  const size_t bufSize = (_appSendBuffer == 0 ? len : _appSendBuffer);

  char buf[bufSize];

  size_t totalSent = 0;
  ssize_t sent;

  bool eagain;

  do {
    sent = Socket::Write(buf, std::min(len - totalSent, bufSize), eagain);

    if (sent > 0) {
      totalSent += sent;
    } else if ((sent == 0) || ((sent == -1) && !eagain)) {
      break;
    }
  } while (totalSent < len);

  return (totalSent == len);
}

auto TestSocket::SetAppRcvBuf(uint32_t appReceiveBuffer) -> void {
  _appReceiveBuffer = appReceiveBuffer;
}

auto TestSocket::SetAppSndBuf(uint32_t appSendBuffer) -> void {
  _appSendBuffer = appSendBuffer;
}
