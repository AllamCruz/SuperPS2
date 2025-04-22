#ifndef SUPERPS2_EE_MEMORY_H
#define SUPERPS2_EE_MEMORY_H

#include <cstdint>

namespace ee {
    namespace memory {
        // Inicialização e liberação
        bool init();
        void shutdown();
        void reset();

        // Acesso genérico de leitura
        uint8_t  read8(uint32_t addr);
        uint16_t read16(uint32_t addr);
        uint32_t read32(uint32_t addr);
        uint64_t read64(uint32_t addr);

        // Acesso genérico de escrita
        void write8(uint32_t addr, uint8_t value);
        void write16(uint32_t addr, uint16_t value);
        void write32(uint32_t addr, uint32_t value);
        void write64(uint32_t addr, uint64_t value);
    }
}

#endif // SUPERPS2_EE_MEMORY_H 