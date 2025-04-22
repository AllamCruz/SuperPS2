#ifndef SUPERPS2_SCRATCHPAD_H
#define SUPERPS2_SCRATCHPAD_H

#include <cstdint>

namespace scratchpad {
    // Inicializa o scratchpad (8 KB) — chamar em memory::init()
    bool init();

    // Leitura de 8/16/32 bits no offset dentro do scratchpad
    uint8_t  read8(uint32_t offset);
    uint16_t read16(uint32_t offset);
    uint32_t read32(uint32_t offset);

    // Escrita de 8/16/32 bits no offset dentro do scratchpad
    void write8(uint32_t offset, uint8_t value);
    void write16(uint32_t offset, uint16_t value);
    void write32(uint32_t offset, uint32_t value);

    // Libera o buffer
    void shutdown();

    // Verifica se um endereço absoluto pertence ao scratchpad
    bool isScratchpadAddr(uint32_t addr);

    // Converte endereço absoluto em offset dentro do scratchpad
    uint32_t addrToOffset(uint32_t addr);
}

#endif // SUPERPS2_SCRATCHPAD_H 