#include "producer.h"

#include "memory_map.h"

#include <cstdint>
#include <iostream>

#include <opencv2/opencv.hpp>

Producer::Producer(sc_core::sc_module_name name, const std::string& input_path)
	: sc_core::sc_module(name), socket("socket"), input_path_(input_path) {
	SC_THREAD(run);
}

void Producer::write_u32(sc_dt::uint64 addr, std::uint32_t value) {
	tlm::tlm_generic_payload tx;
	sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

	tx.set_command(tlm::TLM_WRITE_COMMAND);
	tx.set_address(addr);
	tx.set_data_ptr(reinterpret_cast<unsigned char*>(&value));
	tx.set_data_length(4);
	tx.set_streaming_width(4);
	tx.set_byte_enable_ptr(0);
	tx.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

	socket->b_transport(tx, delay);
	wait(delay);

	if (tx.get_response_status() != tlm::TLM_OK_RESPONSE) {
		SC_REPORT_ERROR("Producer", "write_u32 failed");
	}
}

void Producer::write_block(sc_dt::uint64 addr, unsigned char* data, unsigned int len) {
	tlm::tlm_generic_payload tx;
	sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

	tx.set_command(tlm::TLM_WRITE_COMMAND);
	tx.set_address(addr);
	tx.set_data_ptr(data);
	tx.set_data_length(len);
	tx.set_streaming_width(len);
	tx.set_byte_enable_ptr(0);
	tx.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

	socket->b_transport(tx, delay);
	wait(delay);

	if (tx.get_response_status() != tlm::TLM_OK_RESPONSE) {
		SC_REPORT_ERROR("Producer", "write_block failed");
	}
}

void Producer::run() {
	cv::Mat img = cv::imread(input_path_, cv::IMREAD_COLOR);
	if (img.empty()) {
		std::cout << "producer cant read input image\n";
		sc_core::sc_stop();
		return;
	}

	if (!img.isContinuous()) {
		img = img.clone();
	}

	std::uint32_t w = (std::uint32_t)img.cols;
	std::uint32_t h = (std::uint32_t)img.rows;
	std::uint32_t c = (std::uint32_t)img.channels();
	std::uint32_t bytes = w * h * c;

	write_u32(ADDR_WIDTH, w);
	write_u32(ADDR_HEIGHT, h);
	write_u32(ADDR_CHANNELS, c);
	write_u32(ADDR_IMAGE_SIZE, bytes);
	write_block(ADDR_IMAGE_BASE, img.data, bytes);
	write_u32(ADDR_DONE_FLAG, 1);

	std::cout << "producer wrote image at " << sc_core::sc_time_stamp() << "\n";
}
