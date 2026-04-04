#include "producer.h"

#include <iostream>

Producer::Producer(sc_core::sc_module_name name)
    : sc_core::sc_module(name), initiator_socket("initiator_socket") {
    SC_THREAD(generate_frame);
}

void Producer::generate_frame() {
    tlm::tlm_generic_payload payload;
    sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

    for (int y = 0; y < kHeight; ++y) {
        for (int x = 0; x < kWidth; ++x) {
            // 32-bit pixel format: 0x00RRGGBB
            std::uint32_t pixel =
                (static_cast<std::uint32_t>(x % 256) << 16) |
                (static_cast<std::uint32_t>(y % 256) << 8) |
                0x80u;

            payload.set_command(tlm::TLM_WRITE_COMMAND);
            payload.set_address(static_cast<sc_dt::uint64>((y * kWidth + x) * kBytesPerPixel));
            payload.set_data_ptr(reinterpret_cast<unsigned char*>(&pixel));
            payload.set_data_length(kBytesPerPixel);
            payload.set_streaming_width(kBytesPerPixel);
            payload.set_byte_enable_ptr(nullptr);
            payload.set_dmi_allowed(false);
            payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

            initiator_socket->b_transport(payload, delay);

            if (payload.get_response_status() != tlm::TLM_OK_RESPONSE) {
                SC_REPORT_ERROR("Producer", "Transaction failed while sending pixel");
            }
        }
    }

    std::cout << "Producer: finished sending frame at " << sc_core::sc_time_stamp() << "\n";
    sc_core::sc_stop();
}
