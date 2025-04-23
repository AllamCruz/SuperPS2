#ifndef SUPERPS2_MMIO_H
#define SUPERPS2_MMIO_H

#include <cstdint>

namespace SuperPS2 {

// Funções básicas de MMIO
uint32_t read32(uint32_t addr);
void write32(uint32_t addr, uint32_t value);

// Funções para leitura/escrita de 16 bits
uint16_t read16(uint32_t addr);
void write16(uint32_t addr, uint16_t value);

// Funções para leitura/escrita de 8 bits
uint8_t read8(uint32_t addr);
void write8(uint32_t addr, uint8_t value);

// Funções para leitura/escrita de blocos
void readBlock(uint32_t addr, void* buffer, size_t size);
void writeBlock(uint32_t addr, const void* buffer, size_t size);

// Constantes de endereços importantes
constexpr uint32_t RAM_BASE = 0x00000000;
constexpr uint32_t RAM_SIZE = 0x02000000;  // 32MB

constexpr uint32_t BIOS_BASE = 0x1FC00000;
constexpr uint32_t BIOS_SIZE = 0x00400000;  // 4MB

constexpr uint32_t IOP_RAM_BASE = 0x1D000000;
constexpr uint32_t IOP_RAM_SIZE = 0x00200000;  // 2MB

} // namespace SuperPS2

#endif // SUPERPS2_MMIO_H 