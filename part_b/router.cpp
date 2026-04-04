#include "router.h"

Router::Router(sc_core::sc_module_name name)
    : sc_core::sc_module(name), target_socket("target_socket"), initiator_socket("initiator_socket") {
    target_socket.register_b_transport(this, &Router::b_transport);
}

void Router::b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
    // Part B requirement: 10 ns interconnect delay per transaction.
    delay += sc_core::sc_time(10, sc_core::SC_NS);

    // Address decoding hook: in this single-target version we forward all traffic.
    // You can extend this by checking trans.get_address() ranges and dispatching to
    // multiple target sockets.
    initiator_socket->b_transport(trans, delay);
}
