#ifndef SUPERPS2_MMIO_H
#define SUPERPS2_MMIO_H

#include <cstdint>

uint32_t read32(uint32_t addr);
void write32(uint32_t addr, uint32_t value);

#endif // SUPERPS2_MMIO_H
