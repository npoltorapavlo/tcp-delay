#include "testsocket.h"

#include "clientargs.h"

auto main(int argc, char **argv) -> int {
  const ClientArgs args(argc, argv);

  TestSocket socket;

  if (socket.Create() &&

      // setup before connect

      socket.SetRcvBuf(args.receiveBuffer) &&
      socket.SetSndBuf(args.sendBuffer) &&
      ((args.noDelay == 0) || socket.SetNoDelay(args.noDelay)) &&

      // block until connected

      socket.Connect(args.port) &&

      // setup after connect

      socket.SetNonBlocking() &&
      socket.SetRcvBuf(args.connectReceiveBuffer) &&
      socket.SetSndBuf(args.connectSendBuffer)) {

    socket.SetAppRcvBuf(args.appReceiveBuffer);
    socket.SetAppSndBuf(args.appSendBuffer);

    // send and receive continuously

    for (int i = 0; ((i < args.iterations) && socket.Send(args.expectedSendLength) &&
                     socket.Receive(args.expectedReceiveLength)); i++);

  }

  return (0);
}
