#include "consumer.h"

#include <cstring>
#include <fstream>
#include <iostream>

Consumer::Consumer(sc_core::sc_module_name name)
    : sc_core::sc_module(name), target_socket("target_socket") {
    target_socket.register_b_transport(this, &Consumer::b_transport);
    frame_buffer_.resize(static_cast<std::size_t>(kWidth) * static_cast<std::size_t>(kHeight), 0u);
}

void Consumer::b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
    const tlm::tlm_command cmd = trans.get_command();
    const sc_dt::uint64 addr = trans.get_address();
    unsigned char* data_ptr = trans.get_data_ptr();
    const unsigned int len = trans.get_data_length();

    if (cmd != tlm::TLM_WRITE_COMMAND) {
        trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
        return;
    }

    if (len != static_cast<unsigned int>(kBytesPerPixel) || data_ptr == nullptr) {
        trans.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
        return;
    }

    if (addr % kBytesPerPixel != 0u) {
        trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
        return;
    }

    const std::size_t index = static_cast<std::size_t>(addr / kBytesPerPixel);
    if (index >= frame_buffer_.size()) {
        trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
        return;
    }

    std::memcpy(&frame_buffer_[index], data_ptr, kBytesPerPixel);

    // Part B requirement: 50 ns memory write latency per transaction.
    delay += sc_core::sc_time(50, sc_core::SC_NS);
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
}

void Consumer::save_image(const std::string& filename) const {
    std::ofstream ofs(filename, std::ios::binary);
    ofs << "P6\n" << kWidth << " " << kHeight << "\n255\n";

    for (std::uint32_t pixel : frame_buffer_) {
        const unsigned char rgb[3] = {
            static_cast<unsigned char>((pixel >> 16) & 0xFFu),
            static_cast<unsigned char>((pixel >> 8) & 0xFFu),
            static_cast<unsigned char>(pixel & 0xFFu),
        };
        ofs.write(reinterpret_cast<const char*>(rgb), 3);
    }

    std::cout << "Consumer: wrote " << filename << "\n";
}
