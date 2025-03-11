cmake_minimum_required(VERSION 3.30)
project(Test)

set(CMAKE_CXX_STANDARD 20)

add_executable(Test main.cpp
        server.cpp
        server.h
        Socket.cpp
        Socket.h
        Request.cpp
        Request.h
        Response.cpp
        Response.h
)
