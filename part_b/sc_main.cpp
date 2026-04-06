#include <iostream>

#include <systemc>

#include "consumer.h"
#include "producer.h"
#include "router.h"

int sc_main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // Create modules.
    Producer producer("producer");
    Router router("router");
    Consumer consumer("consumer");

    // Connect Producer -> Router -> Consumer.
    producer.initiator_socket.bind(router.target_socket);
    router.initiator_socket.bind(consumer.target_socket);

    // Expected end time = pixels * (10ns router + 50ns memory).
    int w = 780;
    int h = 610;
    long long pixels = static_cast<long long>(w) * h;
    long long expected_ns = pixels * (10 + 50);

    std::cout << "start time: 0 ns\n";
    std::cout << "expected end time: " << expected_ns << " ns\n";

    // Run simulation.
    sc_core::sc_start();

    std::cout << "end time (actual): "
              << sc_core::sc_time_stamp().to_string() << "\n";

    // Save received frame for visual check.
    consumer.save_image("output_frame.ppm");
    return 0;
}
