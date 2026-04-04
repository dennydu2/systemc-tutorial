#ifndef PART_B_PRODUCER_H
#define PART_B_PRODUCER_H

#include <cstdint>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

class Producer : public sc_core::sc_module {
public:
    tlm_utils::simple_initiator_socket<Producer> initiator_socket;

    SC_HAS_PROCESS(Producer);
    explicit Producer(sc_core::sc_module_name name);

private:
    static constexpr int kWidth = 780;
    static constexpr int kHeight = 610;
    static constexpr int kBytesPerPixel = 4;

    void generate_frame();
};

#endif
