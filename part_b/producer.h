#ifndef PART_B_PRODUCER_H
#define PART_B_PRODUCER_H

#include <cstdint>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

// Producer sends one pixel per TLM write transaction.
class Producer : public sc_core::sc_module {
public:
    tlm_utils::simple_initiator_socket<Producer> initiator_socket;

    SC_HAS_PROCESS(Producer);
    explicit Producer(sc_core::sc_module_name name);

private:
    // Frame size and 32-bit pixel width in bytes.
    static const int WIDTH = 780;
    static const int HEIGHT = 610;
    static const int BYTES = 4;

    // Main generator thread.
    void generate_frame();
};

#endif
