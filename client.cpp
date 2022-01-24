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
  int defaultIterations = 1;
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
        iterations((argc > 9) ? strtol(argv[9], nullptr, 10) : defaultIterations) {
  }

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

auto main(int argc, char **argv) -> int {
  const Args args(argc, argv);

  TestSocket socket;

  if (socket.Create() &&
      ((args.noDelay == 0) || socket.SetNoDelay(args.noDelay)) &&
      socket.Connect(args.port) && // block until connected
      socket.SetNonBlocking() &&
      socket.SetRcvBuf(args.receiveBuffer) &&
      socket.SetSndBuf(args.sendBuffer)) {

    socket.SetAppRcvBuf(args.appReceiveBuffer);
    socket.SetAppSndBuf(args.appSendBuffer);

    // send and receive continuously

    for (int i = 0; ((i < args.iterations) && socket.Send(args.expectedSendLength) &&
                     socket.Receive(args.expectedReceiveLength)); i++);

  }

  return (0);
}
