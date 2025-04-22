#ifndef SUPERPS2_MEMORY_H
#define SUPERPS2_MEMORY_H

#include <cstdint>

namespace ee {
    namespace memory {

        void initMemory();
        void shutdownMemory();

        uint32_t read32(uint32_t addr);
        void write32(uint32_t addr, uint32_t value);

        uint8_t* getPointer(uint32_t addr);
    }
}

#endif // SUPERPS2_MEMORY_H 