#include "router.h"

Router::Router(sc_core::sc_module_name name)
    : sc_core::sc_module(name), target_socket("target_socket"), initiator_socket("initiator_socket") {
    // Register target callback.
    target_socket.register_b_transport(this, &Router::b_transport);
}

void Router::b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
    // Part B timing: add 10ns in router.
    delay += sc_core::sc_time(10, sc_core::SC_NS);

    // Basic address decode/range check.
    sc_dt::uint64 addr = trans.get_address();
    if (addr < MEM_START || addr >= (MEM_START + MEM_SIZE)) {
        trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
        return;
    }

    // Forward transaction to memory with local offset.
    trans.set_address(addr - MEM_START);
    initiator_socket->b_transport(trans, delay);
}
