#ifndef PART_B_ROUTER_H
#define PART_B_ROUTER_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

class Router : public sc_core::sc_module {
public:
    tlm_utils::simple_target_socket<Router> target_socket;
    tlm_utils::simple_initiator_socket<Router> initiator_socket;

    SC_HAS_PROCESS(Router);
    explicit Router(sc_core::sc_module_name name);

private:
    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);
};

#endif
