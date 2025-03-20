cmake_minimum_required(VERSION 3.30)
project(Test)

set(CMAKE_CXX_STANDARD 20)

add_executable(Test main.cpp
        socket/SSocket.cpp
        socket/SSocket.h
        http/Request.cpp
        http/Request.h
        http/Response.cpp
        http/Response.h
        socket/SocketArgs.h
)
