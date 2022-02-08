add_executable(client client.cpp clientargs.cpp socket.cpp testsocket.cpp timestamp.cpp)
add_executable(server server.cpp serverargs.cpp socket.cpp testsocket.cpp timestamp.cpp)

install(TARGETS DESTINATION bin)
