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

    const int width = 780;
    const int height = 610;
    const long long total_pixels = static_cast<long long>(width) * static_cast<long long>(height);
    const long long per_txn_ns = 10 + 50;
    const long long expected_ns = total_pixels * per_txn_ns;

    std::cout << "Simulation started\n";
    std::cout << "Expected frame transfer time: " << expected_ns << " ns ("
              << (expected_ns / 1000000.0) << " ms)\n";

    sc_core::sc_start();

    std::cout << "Simulation finished at " << sc_core::sc_time_stamp() << "\n";

    consumer.save_image("output_frame.ppm");
    return 0;
}
