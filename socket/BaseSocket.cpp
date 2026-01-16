//
// Created by b1n4r33 on 3/10/25.
//

#include "BaseSocket.h"
#include <iostream>
#include "WinSockManager.h"

#ifdef linux
    // Linux specific networking APIs
    #include <cstring>
    #include <ostream>
    #include <sys/socket.h>
    #include <fcntl.h>
#endif
#ifdef _WIN32
    // Windows specific networking

    #include <ws2tcpip.h>

#endif


namespace Waiter {






}




