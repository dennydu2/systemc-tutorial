#ifndef PART_B_ROUTER_H
#define PART_B_ROUTER_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <cstdint>

// Router sits between producer and consumer.
class Router : public sc_core::sc_module {
public:
    tlm_utils::simple_target_socket<Router> target_socket;
    tlm_utils::simple_initiator_socket<Router> initiator_socket;

    SC_HAS_PROCESS(Router);
    explicit Router(sc_core::sc_module_name name);

private:
    // Valid address window for frame memory.
    static const std::uint64_t MEM_START = 0;
    static const std::uint64_t MEM_SIZE = 780ULL * 610ULL * 4ULL;

    // Router transport callback.
    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);
};

#endif
