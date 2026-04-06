#include "memory.h"

#include <cstring>

Memory::Memory(sc_core::sc_module_name name, std::size_t bytes)
	: sc_core::sc_module(name), socket_p1("socket_p1"), socket_p2("socket_p2"), mem(bytes, 0) {
	socket_p1.register_b_transport(this, &Memory::b_transport);
	socket_p2.register_b_transport(this, &Memory::b_transport);
}

void Memory::b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
	sc_dt::uint64 addr = trans.get_address();
	unsigned int len = trans.get_data_length();
	unsigned char* ptr = trans.get_data_ptr();

	if (ptr == 0 || (addr + len) > mem.size()) {
		trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
		return;
	}

	if (trans.get_command() == tlm::TLM_WRITE_COMMAND) {
		std::memcpy(&mem[(std::size_t)addr], ptr, len);
	} else if (trans.get_command() == tlm::TLM_READ_COMMAND) {
		std::memcpy(ptr, &mem[(std::size_t)addr], len);
	} else {
		trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
		return;
	}

	delay += sc_core::sc_time(5, sc_core::SC_NS);
	trans.set_response_status(tlm::TLM_OK_RESPONSE);
}
