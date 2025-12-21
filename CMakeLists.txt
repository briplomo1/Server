cmake_minimum_required(VERSION 3.30)
project(Test)

set(CMAKE_CXX_STANDARD 20)

add_executable(Test
        socket/SocketArgs.h
        socket/Socket.cpp
        socket/Socket.h
        socket/WinSockManager.cpp
        socket/WinSockManager.h
        http/Request.cpp
        http/Request.h
        http/Response.cpp
        http/Response.h
        net/NetworkingInterface.h
)
