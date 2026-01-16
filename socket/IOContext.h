//
// Created by bripl on 1/8/2026.
//

#ifndef IOCONTEXT_H
#define IOCONTEXT_H
#include <atomic>
#include "BaseSocket.h"
#include "EventLoop.h"

namespace Waiter
{
    class IOContext {

        std::atomic<int> ref_count{0};

        std::vector<SocketDescriptor> sockets;

        EventLoop& loop;


    public:

        static IOContext& instance();

        void attach_socket(BaseSocket* socket); // Move? reference?

        void detach_socket(BaseSocket* socket);

        void close_socket(BaseSocket& socket);

        void post_task(Task task);

        EventLoop& get_loop();

    private:

        IOContext();
        ~IOContext();

        void run_loop();

        void stop_if_idle();

        void check_running();

    };
}



#endif //IOCONTEXT_H
