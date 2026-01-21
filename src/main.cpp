//
// Created by bripl on 1/2/2026.
//
#include <waiter/waiter.h>



int main(int argc, char **argv) {
    const Waiter::Endpoint<Waiter::IPV4, Waiter::STREAM, Waiter::PROTOCOL::DEFAULT, false> ep("127.0.0.1", 8080);

    class MySock : Waiter::BaseSocket<Waiter::IPV4, Waiter::STREAM, Waiter::PROTOCOL::DEFAULT, false, false> {
        void handle_error() override {}

        void handle_read() override {}

        void handle_write() override {}
    };

    const MySock socket;





}