#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <thread>

auto socket_recv(int fd, const size_t len) -> size_t
{
    size_t result = 0;
    char buf[len];
    while (result < len) {
        ssize_t size = ::recv(fd, buf, len, 0);
        if (size > 0) {
            result += size;
        } else if ((size == 0) || ((errno != EAGAIN) && (errno != EWOULDBLOCK))) {
            break;
        }
    }
    return result;
}

auto socket_send(int fd, const size_t len, const size_t packet) -> size_t
{
    size_t result = 0;
    char buf[len];
    while (result < len) {
        ssize_t size = ::send(fd, buf, std::min(len - result, packet), 0);
        if (size > 0) {
            result += size;
        } else if ((size == 0) || ((errno != EAGAIN) && (errno != EWOULDBLOCK))) {
            break;
        }
    }
    return result;
}

struct Args {
    Args(int argc, char** argv)
        : port(strtoul(argv[1], nullptr, 10))
        , reuse(strtol(argv[2], nullptr, 10))
        , backlog(strtol(argv[3], nullptr, 10))
        , receiveBuffer(strtoul(argv[4], nullptr, 10))
        , sendBuffer(strtoul(argv[5], nullptr, 10))
        , messageSize(strtoul(argv[6], nullptr, 10))
        , responseSize(strtoul(argv[7], nullptr, 10))
        , packetSize(strtoul(argv[8], nullptr, 10))
    {
    }

    uint16_t port;
    int reuse;
    int backlog;
    uint32_t receiveBuffer;
    uint32_t sendBuffer;
    size_t messageSize;
    size_t responseSize;
    size_t packetSize;
};

auto main(int argc, char** argv) -> int
{
    const Args args(argc, argv);

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(args.port);

    std::thread server([&]() {
        int fd;
        int new_fd;
        struct sockaddr address{};
        socklen_t addrlen = sizeof(address);

        if (((fd = ::socket(AF_INET, SOCK_STREAM, 0)) == -1)
            || (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &args.reuse, sizeof(args.reuse)) == -1)
            || (::bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
            || (::listen(fd, args.backlog) == -1)
            || ((new_fd = ::accept(fd, &address, (socklen_t*)&addrlen)) == -1)
            || (::fcntl(new_fd, F_SETFL, fcntl(new_fd, F_GETFL, 0) | O_NONBLOCK) != 0)
            || (::setsockopt(new_fd, SOL_SOCKET, SO_RCVBUF, &args.receiveBuffer, sizeof(args.receiveBuffer)) == -1)) {
            printf("err %d %s\n", errno, strerror(errno));
        } else {
            while ((socket_recv(new_fd, args.messageSize) == args.messageSize)
                && (socket_send(new_fd, args.responseSize, args.packetSize) == args.responseSize))
                ;
        }
    });

    std::thread client([&]() {
        int fd;

        if (((fd = ::socket(AF_INET, SOCK_STREAM, 0)) == -1)
            || (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &args.reuse, sizeof(args.reuse)) == -1)
            || (::connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
            || (::fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK) != 0)
            || (::setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &args.sendBuffer, sizeof(args.sendBuffer)) == -1)) {
            printf("err %d %s\n", errno, strerror(errno));
        } else {
            while ((socket_send(fd, args.messageSize, args.packetSize) == args.messageSize)
                && (socket_recv(fd, args.responseSize) == args.responseSize))
                ;
        }
    });

    server.join();
    client.join();

    return (0);
}
