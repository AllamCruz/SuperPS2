#pragma once

#include <stdint.h>

namespace ee {
    namespace memory {

        constexpr uint32_t EE_RAM_SIZE = 32 * 1024 * 1024; // 32MB
        constexpr uint32_t EE_RAM_MASK = EE_RAM_SIZE - 1;

        // Memory regions
        constexpr uint32_t EE_RAM_START = 0x00000000;
        constexpr uint32_t EE_RAM_END = 0x01FFFFFF;
        
        // Memory mirrors
        constexpr uint32_t EE_RAM_MIRROR1_START = 0x20000000;
        constexpr uint32_t EE_RAM_MIRROR1_END = 0x21FFFFFF;
        constexpr uint32_t EE_RAM_MIRROR2_START = 0x40000000;
        constexpr uint32_t EE_RAM_MIRROR2_END = 0x41FFFFFF;
        constexpr uint32_t EE_RAM_MIRROR3_START = 0x60000000;
        constexpr uint32_t EE_RAM_MIRROR3_END = 0x61FFFFFF;

        bool init();
        void reset();
        void shutdown();

        // Basic memory access
        uint8_t read8(uint32_t addr);
        uint16_t read16(uint32_t addr);
        uint32_t read32(uint32_t addr);
        uint64_t read64(uint32_t addr);

        void write8(uint32_t addr, uint8_t value);
        void write16(uint32_t addr, uint16_t value);
        void write32(uint32_t addr, uint32_t value);
        void write64(uint32_t addr, uint64_t value);

        // Direct memory access for DMA and special cases
        uint8_t* getRawMemory();
        bool isAddressValid(uint32_t addr);
        uint32_t translateAddress(uint32_t addr);

        // Memory protection
        void protectRegion(uint32_t start, uint32_t end);
        void unprotectRegion(uint32_t start, uint32_t end);
        bool isRegionProtected(uint32_t addr);

        // Cache control
        void invalidateICache();
        void invalidateDCache();
        void writebackDCache();
    }
} 