#ifndef SECTIONB_LINE_PROCESSOR_H
#define SECTIONB_LINE_PROCESSOR_H

#include <cstdint>
#include <string>

#include <opencv2/core.hpp>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

class LineProcessor : public sc_core::sc_module {
public:
	tlm_utils::simple_initiator_socket<LineProcessor> socket;

	SC_HAS_PROCESS(LineProcessor);
	LineProcessor(sc_core::sc_module_name name, const std::string& output_path);

private:
	std::string output_path_;

	void run();
	std::uint32_t read_u32(sc_dt::uint64 addr);
	void read_block(sc_dt::uint64 addr, unsigned char* data, unsigned int len);
	void maybe_show(const std::string& title, const cv::Mat& img);
};

#endif
