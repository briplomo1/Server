//
// Created by bripl on 1/8/2026.
//

#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include <functional>
#include <queue>
#include "BaseSocket.h"
#include "IOPoller.h"

namespace Waiter {

    enum class State {
        Idle,
        Connecting,
        Reading,
        Writing,
        Closed
    };

    using Task = std::function<void()>;

    class EventLoop {

        std::queue<Task> task_queue;

        bool running{false};

        std::unique_ptr<IOPoller> io_poller;

    public:

        explicit EventLoop(IOPoller& poller);

        void run();

        void stop();

        void post(const Task&& task); // TODO: use lock free ring buffer

        void add_socket(SocketDescriptor fd);

        void remove_socket(SocketDescriptor fd);

        ~EventLoop() = default;

    };


}



#endif //EVENTLOOP_H
