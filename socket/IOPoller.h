//
// Created by bripl on 1/8/2026.
//

#ifndef IOPOLLER_H
#define IOPOLLER_H
#include "BaseSocket.h"

namespace Waiter {

    struct PollEvent {
        void *userData;
        uint32_t mask; // ?
    };

    class IOPoller {

        std::vector<std::unique_ptr<BaseSocket>> sockets;

    public:
        virtual ~IOPoller() = default;

        virtual void add(SocketDescriptor fd, uint32_t events, void* userData) = 0;

        virtual void modify(SocketDescriptor fd, uint32_t events, void* userData) = 0;

        virtual void remove(SocketDescriptor fd) = 0;

        virtual int poll(PollEvent* events, int max_events, int timeout_ms=-1) = 0;

    };

} // Waiter

#endif //IOPOLLER_H
