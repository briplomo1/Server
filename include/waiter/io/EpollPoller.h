//
// Created by bripl on 1/10/2026.
//

#ifndef EPOLLPOLLER_H
#define EPOLLPOLLER_H
#include "IOPoller.h"

namespace Waiter
{
    class EpollPoller final : public IOPoller {
        int epoll_fd;

    public:

        EpollPoller();

        ~EpollPoller() override;

        void add(SocketDescriptor fd, uint32_t events, void* userData) override;

        void modify(SocketDescriptor fd, uint32_t events, void* userData) override;

        void remove(SocketDescriptor fd) override;

        int wait(PollEvent* events, int max_events, int timeout_ms=-1) override;

    };
}

#endif //EPOLLPOLLER_H
