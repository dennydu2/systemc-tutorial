#include "consumer.h"

#include <cstring>
#include <fstream>
#include <iostream>

Consumer::Consumer(sc_core::sc_module_name name)
    : sc_core::sc_module(name), target_socket("target_socket") {
    target_socket.register_b_transport(this, &Consumer::b_transport);
    mem.resize(WIDTH * HEIGHT, 0);
}

void Consumer::b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
    sc_dt::uint64 addr = trans.get_address();
    unsigned char* ptr = trans.get_data_ptr();

    if (trans.get_command() != tlm::TLM_WRITE_COMMAND) {
        trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
        return;
    }

    if (trans.get_data_length() != BYTES || ptr == 0) {
        trans.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
        return;
    }

    if (addr % BYTES != 0) {
        trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
        return;
    }

    unsigned int index = static_cast<unsigned int>(addr / BYTES);
    if (index >= mem.size()) {
        trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
        return;
    }

    std::memcpy(&mem[index], ptr, BYTES);

    // Part B asks for 50ns at memory side
    delay += sc_core::sc_time(50, sc_core::SC_NS);
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
}

void Consumer::save_image(const std::string& filename) const {
    std::ofstream ofs(filename, std::ios::binary);
    ofs << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";

    for (std::uint32_t pixel : mem) {
        const unsigned char rgb[3] = {
            static_cast<unsigned char>((pixel >> 16) & 0xFFu),
            static_cast<unsigned char>((pixel >> 8) & 0xFFu),
            static_cast<unsigned char>(pixel & 0xFFu),
        };
        ofs.write(reinterpret_cast<const char*>(rgb), 3);
    }

    std::cout << "saved image: " << filename << "\n";
}
