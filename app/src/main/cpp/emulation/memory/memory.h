#ifndef SUPERPS2_MEMORY_H
#define SUPERPS2_MEMORY_H

#include <cstdint>
#include <cstddef>

namespace memory {
    bool init();                      // Inicializa e zera a memória RAM principal
    void shutdown();                  // Libera a memória

    uint8_t  read8(uint32_t addr);    // Leitura de 8 bits
    uint16_t read16(uint32_t addr);   // Leitura de 16 bits
    uint32_t read32(uint32_t addr);   // Leitura de 32 bits

    void write8(uint32_t addr, uint8_t value);      // Escrita de 8 bits
    void write16(uint32_t addr, uint16_t value);    // Escrita de 16 bits
    void write32(uint32_t addr, uint32_t value);    // Escrita de 32 bits

    bool isValidAddress(uint32_t addr);             // Verifica se o endereço está dentro da RAM
}

#endif //SUPERPS2_MEMORY_H
