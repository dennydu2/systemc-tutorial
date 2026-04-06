#ifndef SECTIONB_PRODUCER_H
#define SECTIONB_PRODUCER_H

#include <cstdint>
#include <string>

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

class Producer : public sc_core::sc_module {
public:
	tlm_utils::simple_initiator_socket<Producer> socket;

	SC_HAS_PROCESS(Producer);
	Producer(sc_core::sc_module_name name, const std::string& input_path);

private:
	std::string input_path_;

	void run();
	void write_u32(sc_dt::uint64 addr, std::uint32_t value);
	void write_block(sc_dt::uint64 addr, unsigned char* data, unsigned int len);
};

#endif
