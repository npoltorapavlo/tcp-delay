project(tcp-delay)

cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_STANDARD 11)

add_executable(client client.cpp socket.cpp testsocket.cpp timestamp.cpp)
add_executable(server server.cpp socket.cpp testsocket.cpp timestamp.cpp)

install(TARGETS DESTINATION bin)
