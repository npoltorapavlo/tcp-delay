project(tcp-delay)

cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} tcp-delay.cpp)

install(TARGETS DESTINATION bin)
