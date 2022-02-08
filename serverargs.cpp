#include "serverargs.h"

#include <cstdlib>

namespace {
  uint16_t defaultPort = 55555;
  size_t defaultReceiveBuffer = 1;
  size_t defaultSendBuffer = 1;
  size_t defaultAcceptReceiveBuffer = 1;
  size_t defaultAcceptSendBuffer = 1;
  size_t defaultAppReceiveBuffer = 1;
  size_t defaultAppSendBuffer = 1;
  size_t defaultExpectedReceiveLength = 1;
  size_t defaultExpectedSendLength = 1;
  int defaultNoDelay = 0;
  int defaultBacklog = 1;
}

ServerArgs::ServerArgs(int argc, char **argv)
    : port((argc > 1) ? strtoul(argv[1], nullptr, 10) : defaultPort),
      receiveBuffer((argc > 2) ? strtoul(argv[2], nullptr, 10) : defaultReceiveBuffer),
      sendBuffer((argc > 3) ? strtoul(argv[3], nullptr, 10) : defaultSendBuffer),
      acceptReceiveBuffer((argc > 4) ? strtoul(argv[4], nullptr, 10) : defaultAcceptReceiveBuffer),
      acceptSendBuffer((argc > 5) ? strtoul(argv[5], nullptr, 10) : defaultAcceptSendBuffer),
      appReceiveBuffer((argc > 6) ? strtoul(argv[6], nullptr, 10) : defaultAppReceiveBuffer),
      appSendBuffer((argc > 7) ? strtoul(argv[7], nullptr, 10) : defaultAppSendBuffer),
      expectedReceiveLength((argc > 8) ? strtoul(argv[8], nullptr, 10) : defaultExpectedReceiveLength),
      expectedSendLength((argc > 9) ? strtoul(argv[9], nullptr, 10) : defaultExpectedSendLength),
      noDelay((argc > 10) ? strtol(argv[10], nullptr, 10) : defaultNoDelay),
      backlog((argc > 11) ? strtol(argv[11], nullptr, 10) : defaultBacklog) {
}
