cmake_minimum_required(VERSION 3.30)
project(Test)

set(CMAKE_CXX_STANDARD 20)

add_library(waiter STATIC
        src/io/WinSockManager.cpp
        src/io/EpollPoller.cpp
        src/io/IOContext.cpp
)

target_include_directories(waiter
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)


add_executable(Test
    src/main.cpp
)

target_link_libraries(Test
    PRIVATE waiter
)

if (WIN32)
    target_link_libraries(Test PRIVATE Ws2_32)

endif ()


