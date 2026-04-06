#ifndef PART_B_CONSUMER_H
#define PART_B_CONSUMER_H

#include <cstdint>
#include <string>
#include <vector>

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

// Consumer receives TLM writes and stores pixels in memory.
class Consumer : public sc_core::sc_module {
public:
    tlm_utils::simple_target_socket<Consumer> target_socket;

    SC_HAS_PROCESS(Consumer);
    explicit Consumer(sc_core::sc_module_name name);

    void save_image(const std::string& filename) const;
private:
    // Frame size and 32-bit pixel width in bytes.
    static const int WIDTH = 780;
    static const int HEIGHT = 610;
    static const int BYTES = 4;

    // Local memory where incoming pixels are stored.
    std::vector<std::uint32_t> mem;

    // Handles incoming TLM transactions.
    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);
};

#endif
