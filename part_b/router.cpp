#include "router.h"

Router::Router(sc_core::sc_module_name name)
    : sc_core::sc_module(name), target_socket("target_socket"), initiator_socket("initiator_socket") {
    target_socket.register_b_transport(this, &Router::b_transport);
}

void Router::b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
    // 10ns router delay from assignment
    delay += sc_core::sc_time(10, sc_core::SC_NS);

    sc_dt::uint64 addr = trans.get_address();
    if (addr < MEM_START || addr >= (MEM_START + MEM_SIZE)) {
        trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
        return;
    }

    // map to local memory offset then forward
    trans.set_address(addr - MEM_START);
    initiator_socket->b_transport(trans, delay);
}
