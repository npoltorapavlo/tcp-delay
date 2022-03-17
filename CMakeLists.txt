project(tcp-delay)

# git clean -fdx && cmake . && make

cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_STANDARD 11)

add_executable(client
        Source/client.cpp
        Source/clientargs.cpp
        Source/socket.cpp
        Source/testsocket.cpp
        Source/timestamp.cpp
        )

add_executable(server
        Source/server.cpp
        Source/serverargs.cpp
        Source/socket.cpp
        Source/testsocket.cpp
        Source/timestamp.cpp)

install(TARGETS DESTINATION bin)
