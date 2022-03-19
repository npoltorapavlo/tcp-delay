#pragma once

#include <cstdlib>

struct ServerArgs {
    ServerArgs(int argc, char** argv)
        : port(strtoul(argv[1], nullptr, 10))
        , receiveBuffer(strtoul(argv[2], nullptr, 10))
        , sendBuffer(strtoul(argv[3], nullptr, 10))
        , acceptReceiveBuffer(strtoul(argv[4], nullptr, 10))
        , acceptSendBuffer(strtoul(argv[5], nullptr, 10))
        , appReceiveBuffer(strtoul(argv[6], nullptr, 10))
        , appSendBuffer(strtoul(argv[7], nullptr, 10))
        , expectedReceiveLength(strtoul(argv[8], nullptr, 10))
        , expectedSendLength(strtoul(argv[9], nullptr, 10))
        , noDelay(strtol(argv[10], nullptr, 10))
        , backlog(strtol(argv[11], nullptr, 10))
    {
    }

    uint16_t port;
    size_t receiveBuffer;
    size_t sendBuffer;
    size_t acceptReceiveBuffer;
    size_t acceptSendBuffer;
    size_t appReceiveBuffer;
    size_t appSendBuffer;
    size_t expectedReceiveLength;
    size_t expectedSendLength;
    int noDelay;
    int backlog;
};
