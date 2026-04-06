#include "line_processor.h"

#include "memory_map.h"

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

LineProcessor::LineProcessor(sc_core::sc_module_name name, const std::string& output_path)
	: sc_core::sc_module(name), socket("socket"), output_path_(output_path) {
	SC_THREAD(run);
}

std::uint32_t LineProcessor::read_u32(sc_dt::uint64 addr) {
	std::uint32_t value = 0;
	tlm::tlm_generic_payload tx;
	sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

	tx.set_command(tlm::TLM_READ_COMMAND);
	tx.set_address(addr);
	tx.set_data_ptr(reinterpret_cast<unsigned char*>(&value));
	tx.set_data_length(4);
	tx.set_streaming_width(4);
	tx.set_byte_enable_ptr(0);
	tx.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

	socket->b_transport(tx, delay);
	wait(delay);

	if (tx.get_response_status() != tlm::TLM_OK_RESPONSE) {
		SC_REPORT_ERROR("LineProcessor", "read_u32 failed");
	}

	return value;
}

void LineProcessor::read_block(sc_dt::uint64 addr, unsigned char* data, unsigned int len) {
	tlm::tlm_generic_payload tx;
	sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

	tx.set_command(tlm::TLM_READ_COMMAND);
	tx.set_address(addr);
	tx.set_data_ptr(data);
	tx.set_data_length(len);
	tx.set_streaming_width(len);
	tx.set_byte_enable_ptr(0);
	tx.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

	socket->b_transport(tx, delay);
	wait(delay);

	if (tx.get_response_status() != tlm::TLM_OK_RESPONSE) {
		SC_REPORT_ERROR("LineProcessor", "read_block failed");
	}
}

void LineProcessor::maybe_show(const std::string& title, const cv::Mat& img) {
	const char* display = std::getenv("DISPLAY");
	if (display && display[0] != '\0') {
		cv::imshow(title, img);
		cv::waitKey(1);
	}
}

void LineProcessor::run() {
	while (read_u32(ADDR_DONE_FLAG) == 0) {
		wait(sc_core::sc_time(100, sc_core::SC_US));
	}

	std::uint32_t w = read_u32(ADDR_WIDTH);
	std::uint32_t h = read_u32(ADDR_HEIGHT);
	std::uint32_t c = read_u32(ADDR_CHANNELS);
	std::uint32_t bytes = read_u32(ADDR_IMAGE_SIZE);

	if (w == 0 || h == 0 || c != 3 || bytes != (w * h * c)) {
		SC_REPORT_ERROR("LineProcessor", "metadata is wrong");
		sc_core::sc_stop();
		return;
	}

	std::vector<unsigned char> raw(bytes);
	read_block(ADDR_IMAGE_BASE, raw.data(), bytes);

	cv::Mat input((int)h, (int)w, CV_8UC3, raw.data());
	cv::Mat img = input.clone();

	cv::Mat gray;
	cv::Mat blur_img;
	cv::Mat edges;
	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(gray, blur_img, cv::Size(5, 5), 1.5);
	cv::Canny(blur_img, edges, 50, 150);

	cv::Mat roi_mask = cv::Mat::zeros(edges.size(), edges.type());
	cv::Point pts[4];
	pts[0] = cv::Point((int)(0.05 * w), (int)h);
	pts[1] = cv::Point((int)(0.45 * w), (int)(0.55 * h));
	pts[2] = cv::Point((int)(0.55 * w), (int)(0.55 * h));
	pts[3] = cv::Point((int)(0.95 * w), (int)h);
	cv::fillConvexPoly(roi_mask, pts, 4, cv::Scalar(255));

	cv::Mat road_edges;
	cv::bitwise_and(edges, roi_mask, road_edges);

	std::vector<cv::Vec4i> lines;
	cv::HoughLinesP(road_edges, lines, 1, CV_PI / 180, 40, 35, 20);

	// Build output like the sample: white edges on black, then draw red lines.
	cv::Mat out;
	cv::cvtColor(edges, out, cv::COLOR_GRAY2BGR);
	for (size_t i = 0; i < lines.size(); i++) {
		cv::Vec4i l = lines[i];
		double dx = (double)l[2] - (double)l[0];
		double dy = (double)l[3] - (double)l[1];

		if (std::fabs(dx) < 1e-6) {
			continue;
		}

		double slope = dy / dx;
		if (std::fabs(slope) < 0.3) {
			continue;
		}

		if (l[1] < (int)(0.45 * h) || l[3] < (int)(0.45 * h)) {
			continue;
		}

		cv::line(out, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 3);
	}

	cv::imwrite(output_path_, out);
	std::cout << "line output saved: " << output_path_ << "\n";
	maybe_show("line", out);
	sc_core::sc_stop();
}
