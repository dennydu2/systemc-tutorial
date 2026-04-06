#include <iostream>
#include <string>

#include <systemc>

#include "line_processor.h"
#include "memory.h"
#include "producer.h"

int sc_main(int argc, char* argv[]) {
	std::string input = "input.jpg";
	std::string output = "line_output.jpg";

	if (argc > 1) {
		input = argv[1];
	}
	if (argc > 2) {
		output = argv[2];
	}

	Memory mem("mem", 30 * 1024 * 1024);
	Producer p1("producer", input);
	LineProcessor p2("line_processor", output);

	p1.socket.bind(mem.socket_p1);
	p2.socket.bind(mem.socket_p2);

	std::cout << "start systemc\n";
	sc_core::sc_start();
	std::cout << "done at " << sc_core::sc_time_stamp() << "\n";

	return 0;
}
