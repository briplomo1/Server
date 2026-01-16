cmake_minimum_required(VERSION 3.30)
project(Test)

set(CMAKE_CXX_STANDARD 20)

add_library(waiter STATIC
        socket/SocketTypes.h
        socket/BaseSocket.cpp
        socket/BaseSocket.h
        socket/WinSockManager.cpp
        socket/WinSockManager.h
        socket/EventLoop.cpp
        socket/EventLoop.h
        endpoint/Endpoint.h
        http/Request.cpp
        http/Request.h
        http/Response.cpp
        http/Response.h
        tcp/Server.h
        tcp/Client.h
        tcp/Server.cpp
        tcp/Client.cpp
        socket/EpollPoller.cpp
        socket/IOPoller.h
        socket/IOContext.cpp
        socket/IOContext.h
        socket/EpollPoller.h
)

target_include_directories(waiter
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)


add_executable(Test
    src/main.cpp
)

target_link_libraries(Test PRIVATE waiter)
