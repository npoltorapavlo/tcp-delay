#include "testsocket.h"

#include <cstdlib>

namespace {
  uint16_t defaultPort = 55555;
  size_t defaultReceiveBuffer = 1;
  size_t defaultSendBuffer = 1;
  size_t defaultAppReceiveBuffer = 1;
  size_t defaultAppSendBuffer = 1;
  size_t defaultExpectedReceiveLength = 1;
  size_t defaultExpectedSendLength = 1;
  int defaultNoDelay = 0;
  int defaultBacklog = 1;
}

struct Args {
  Args(int argc, char **argv)
      : port((argc > 1) ? strtoul(argv[1], nullptr, 10) : defaultPort),
        receiveBuffer((argc > 2) ? strtoul(argv[2], nullptr, 10) : defaultReceiveBuffer),
        sendBuffer((argc > 3) ? strtoul(argv[3], nullptr, 10) : defaultSendBuffer),
        appReceiveBuffer((argc > 4) ? strtoul(argv[4], nullptr, 10) : defaultAppReceiveBuffer),
        appSendBuffer((argc > 5) ? strtoul(argv[5], nullptr, 10) : defaultAppSendBuffer),
        expectedReceiveLength((argc > 6) ? strtoul(argv[6], nullptr, 10) : defaultExpectedReceiveLength),
        expectedSendLength((argc > 7) ? strtoul(argv[7], nullptr, 10) : defaultExpectedSendLength),
        noDelay((argc > 8) ? strtol(argv[8], nullptr, 10) : defaultNoDelay),
        backlog((argc > 9) ? strtol(argv[9], nullptr, 10) : defaultBacklog) {
  }

  uint16_t port;
  size_t receiveBuffer;
  size_t sendBuffer;
  size_t appReceiveBuffer;
  size_t appSendBuffer;
  size_t expectedReceiveLength;
  size_t expectedSendLength;
  int noDelay;
  int backlog;
};

auto main(int argc, char **argv) -> int {
  const Args args(argc, argv);

  Socket socket;

  if (socket.Create() &&
      socket.Listen(args.port, args.backlog) && // block until listening
      socket.SetNonBlocking()) {

    int fd;
    bool eagain;

    while (((fd = socket.Accept(eagain)) == -1) && eagain);

    // for this test expecting just one connection

    if (fd != -1) {
      TestSocket new_socket(fd);

      if (((args.noDelay == 0) || new_socket.SetNoDelay(args.noDelay)) &&
          new_socket.SetNonBlocking() &&
          new_socket.SetRcvBuf(args.receiveBuffer) &&
          new_socket.SetSndBuf(args.sendBuffer)) {

        new_socket.SetAppRcvBuf(args.appReceiveBuffer);
        new_socket.SetAppSndBuf(args.appSendBuffer);

        // receive and send continuously

        while (new_socket.Receive(args.expectedReceiveLength) && new_socket.Send(args.expectedSendLength));
      }
    }
  }

  return (0);
}
