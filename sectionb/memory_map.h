#ifndef SECTIONB_MEMORY_MAP_H
#define SECTIONB_MEMORY_MAP_H

#include <cstdint>

static const std::uint64_t ADDR_WIDTH = 0x00;
static const std::uint64_t ADDR_HEIGHT = 0x04;
static const std::uint64_t ADDR_CHANNELS = 0x08;
static const std::uint64_t ADDR_IMAGE_SIZE = 0x0C;
static const std::uint64_t ADDR_DONE_FLAG = 0x10;
static const std::uint64_t ADDR_IMAGE_BASE = 0x100;

#endif
