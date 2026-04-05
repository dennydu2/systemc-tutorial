#include <iostream>

#include <systemc>

#include "consumer.h"
#include "producer.h"
#include "router.h"

int sc_main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    Producer producer("producer");
    Router router("router");
    Consumer consumer("consumer");

    producer.initiator_socket.bind(router.target_socket);
    router.initiator_socket.bind(consumer.target_socket);

    int w = 780;
    int h = 610;
    long long pixels = static_cast<long long>(w) * h;
    long long expected_ns = pixels * (10 + 50);

    std::cout << "start sim\n";
    std::cout << "expected time = " << expected_ns << " ns ("
              << (expected_ns / 1000000.0) << " ms)\n";

    sc_core::sc_start();

    std::cout << "done at " << sc_core::sc_time_stamp() << "\n";

    consumer.save_image("output_frame.ppm");
    return 0;
}
