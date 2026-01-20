//
// Created by bripl on 1/8/2026.
//

#ifndef IOPOLLER_H
#define IOPOLLER_H
#include <cstdint>
#include <bits/unique_ptr.h>

#include "waiter/socket/BaseSocket.h"

namespace Waiter {

    // struct SockCallbacks {
    //     void (*on_readable)(void*);
    //     void (*on_writeable)(void*);
    //     void (*on_error)(void*);
    // };
    //
    // struct SocketEntry {
    //     void* socket;
    //     SockCallbacks callbacks;
    // };
    //
    // class IOPoller {
    //     int epfd;
    //     std::unordered_map<void*, SocketEntry> socket_entries;
    //
    // public:
    //
    //     IOPoller() {
    //         epfd= epoll_create1(0);
    //         if  (epfd == -1) {
    //             perror("epoll_create1 failed");
    //             std::exit(1);
    //         }
    //     }
    //     ~IOPoller() {
    //         close(epfd);
    //     }
    //
    //     template<SocketLike T>
    //     void add_socket(T& socket) {
    //         SockCallbacks callbacks {
    //             [](void* ptr){static_cast<T*>(ptr)->on_readable();},
    //             [](void* ptr){static_cast<T*>(ptr)->on_writeable();},
    //             [](void* ptr){static_cast<T*>(ptr)->on_error();},
    //
    //         };
    //
    //         struct epoll_event ev{};
    //         ev.events = 0;
    //         if (socket.wants_read()) ev.events |= EPOLLIN;
    //         if (socket->wants_write()) ev.events | EPOLLOUT;
    //
    //         ev.data.ptr = &socket;
    //
    //         if (epoll_ctl(epfd, EPOLL_CTL_ADD, socket->get_socket(), &ev) == -1)
    //         {
    //             perror("epoll_ctl ADD socket failed");
    //             std::exit(1);
    //         }
    //         sockets.push_back({&socket, socket.get_socket(), callbacks});
    //     }
    //
    //     template<SocketLike S>
    //     void modify(SocketLike auto& socket);
    //
    //     template<SocketLike S>
    //     void remove_socket(SocketLike auto& socket) {
    //         if (epoll_ctl(epfd, EPOLL_CTL_DEL, socket->get_socket(), nullptr) == -1) {
    //             perror("epoll_ctl DEL socket failed");
    //         }
    //         sockets.erase(socket->get_socket());
    //     }
    //
    //     void poll(int timeout_ms=-1) {
    //         constexpr int MAX_EVENTS = 64;
    //         struct epoll_event events[MAX_EVENTS];
    //
    //         int res = epoll_wait(epfd, events, MAX_EVENTS, timeout_ms);
    //         if (res == -1) {
    //             if (errno == EINTR) return;
    //             perror("epoll_wait failed");
    //             std::exit(1);
    //         }
    //
    //         for (int i = 0; i < res; i++) {
    //             void* sock_ptr = events[i].data.ptr;
    //             int evs = events[i].events;
    //
    //             auto it = socket_entries.find(sock_ptr);
    //             if (it == socket_entries.end()) continue;
    //
    //             auto& sock_cbs = it->second.callbacks;
    //             if (evs & EPOLLIN) sock_cbs.on_readable(sock_ptr);
    //             if (evs & EPOLLOUT) sock_cbs.on_writeable(sock_ptr);
    //             if (evs & EPOLLERR) sock_cbs.on_error(sock_ptr);
    //         }
    //     }
    //
    //     bool empty() const { return socket_entries.empty(); }

    // };

} // Waiter

#endif //IOPOLLER_H
