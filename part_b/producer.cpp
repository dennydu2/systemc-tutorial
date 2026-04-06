#include "producer.h"

#include <iostream>

Producer::Producer(sc_core::sc_module_name name)
    : sc_core::sc_module(name), initiator_socket("initiator_socket") {
    // Start producer thread when simulation starts.
    SC_THREAD(generate_frame);
}

void Producer::generate_frame() {
    tlm::tlm_generic_payload tx;
    sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

    // Scan the full frame and send pixels as transactions.
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            // RGB packed in 32-bit word: 0x00RRGGBB
            std::uint32_t pixel = ((x % 256) << 16) | ((y % 256) << 8) | ((x + y) % 256);
            tx.set_command(tlm::TLM_WRITE_COMMAND);
            tx.set_address((y * WIDTH + x) * BYTES);
            tx.set_data_ptr(reinterpret_cast<unsigned char*>(&pixel));
            tx.set_data_length(BYTES);
            tx.set_streaming_width(BYTES);
            tx.set_byte_enable_ptr(0);
            tx.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

            // Blocking transport call to router/target path.
            delay = sc_core::SC_ZERO_TIME;
            initiator_socket->b_transport(tx, delay);
            wait(delay);

            if (tx.get_response_status() != tlm::TLM_OK_RESPONSE) {
                SC_REPORT_ERROR("Producer", "Transaction failed while sending pixel");
            }
        }
    }

    std::cout << "Producer done at " << sc_core::sc_time_stamp() << "\n";
    // Stop simulation once full frame is sent.
    sc_core::sc_stop();
}
