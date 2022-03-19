#include "testsocket.h"

#include "serverargs.h"

auto main(int argc, char** argv) -> int
{
    const ServerArgs args(argc, argv);

    Socket socket;

    if (socket.Create() &&

        // setup before listen

        socket.SetRcvBuf(args.receiveBuffer) && socket.SetSndBuf(args.sendBuffer) &&

        // block until listening

        socket.Listen(args.port, args.backlog) && socket.SetNonBlocking()) {

        int fd;
        bool eagain;

        while (((fd = socket.Accept(eagain)) == -1) && eagain)
            ;

        // for this test expecting just one connection

        if (fd != -1) {
            TestSocket new_socket(fd);

            // setup after accept

            if (((args.noDelay == 0) || new_socket.SetNoDelay(args.noDelay)) && new_socket.SetNonBlocking() && new_socket.SetRcvBuf(args.acceptReceiveBuffer) && new_socket.SetSndBuf(args.acceptSendBuffer)) {

                new_socket.SetAppRcvBuf(args.appReceiveBuffer);
                new_socket.SetAppSndBuf(args.appSendBuffer);

                // receive and send continuously

                while (new_socket.Receive(args.expectedReceiveLength) && new_socket.Send(args.expectedSendLength))
                    ;
            }
        }
    }

    return (0);
}
