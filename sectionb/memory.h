#ifndef SECTIONB_MEMORY_H
#define SECTIONB_MEMORY_H

#include <vector>

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

class Memory : public sc_core::sc_module {
public:
	tlm_utils::simple_target_socket<Memory> socket_p1;
	tlm_utils::simple_target_socket<Memory> socket_p2;

	SC_HAS_PROCESS(Memory);
	Memory(sc_core::sc_module_name name, std::size_t bytes);

private:
	std::vector<unsigned char> mem;

	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);
};

#endif
