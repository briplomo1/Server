//
// Created by bripl on 1/8/2026.
//

#ifndef IOCONTEXT_H
#define IOCONTEXT_H
#include <atomic>
#include <thread>
#include <waiter/socket/BaseSocket.h>

namespace Waiter {

    /**
     *  SocketLike concept to handle template agnostic operations on BaseSocket
     */
    template<typename T>
    concept SocketLike = requires(T t) {
        {t.get_socket()} -> std::convertible_to<SocketDescriptor>;
        { t.on_readable() } -> std::same_as<void>;
        { t.on_writable() } -> std::same_as<void>;
        { t.on_error() } -> std::same_as<void>;
        { t.on_close() } -> std::same_as<void>;
        { t.wants_read() } -> std::convertible_to<bool>;
        { t.wants_write() } -> std::convertible_to<bool>;
        { t.close_socket() } -> std::same_as<void>;
    };

    template<typename Poller>
    class IOContext {

        using Task = std::function<void()>;
        Poller poller_;
        std::atomic<int> sock_count{0};
        std::atomic<bool> running_{false};
        std::jthread loop_thread_;

        // Only for tasks that run on the event loop
        // Ideally not heavy blocking tasks
        std::queue<Task> tasks;

    public:

        static IOContext& instance() {
            static IOContext ctx;
            return ctx;
        }

        template<SocketLike T>
        void register_socket(const T* socket) {
            poller_->add_socket(socket);
            sock_count.fetch_add(1, std::memory_order_relaxed);
            start_loop();
        }

        template<SocketLike T>
        void deregister_socket(const T* socket) {
            poller_->remove(socket);
            sock_count.fetch_sub(1, std::memory_order_relaxed);
        }

        void post_task(Task task) {
            tasks.push(std::move(task));
            start_loop();
        }

        bool check_running() { return running_; }

    private:

        IOContext() = default;

        ~IOContext() { stop_loop(); }

        void start_loop() {
            if (running_) return;
            running_ = true;
            loop_thread_ = std::jthread([this](std::stop_token stok){loop(stok);});
        }

        void loop(const std::stop_token& stok) {
            while (!stok.stop_requested()) {
                while (!tasks.empty()) {
                    auto task = std::move(tasks.front());
                    tasks.pop();
                    task();
                }
                poller_->poll(10);
                if (poller_.empty() && tasks.empty()) break;
            }
            running_ = false;
        }

        void stop_loop() {
            if (loop_thread_.joinable()) {
                loop_thread_.request_stop();
            }
            running_ = false;
        }



    };
}



#endif //IOCONTEXT_H
