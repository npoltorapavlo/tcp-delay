#include "testsocket.h"

#include "clientargs.h"

auto main(int argc, char **argv) -> int {
  const ClientArgs args(argc, argv);

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
