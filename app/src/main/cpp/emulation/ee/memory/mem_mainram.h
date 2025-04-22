#ifndef SUPERPS2_MEM_MAINRAM_H
#define SUPERPS2_MEM_MAINRAM_H

#include <cstdint>
#include <vector>
#include <functional>

namespace ee {
    namespace memory {

        constexpr uint32_t MAIN_RAM_SIZE = 32 * 1024 * 1024; // 32MB
        constexpr uint32_t MAIN_RAM_MASK = MAIN_RAM_SIZE - 1;

        // Memory regions
        constexpr uint32_t MAIN_RAM_START = 0x00000000;
        constexpr uint32_t MAIN_RAM_END = 0x01FFFFFF;

        // Memory mirrors
        constexpr uint32_t MAIN_RAM_MIRROR1_START = 0x20000000;
        constexpr uint32_t MAIN_RAM_MIRROR1_END = 0x21FFFFFF;
        constexpr uint32_t MAIN_RAM_MIRROR2_START = 0x40000000;
        constexpr uint32_t MAIN_RAM_MIRROR2_END = 0x41FFFFFF;
        constexpr uint32_t MAIN_RAM_MIRROR3_START = 0x60000000;
        constexpr uint32_t MAIN_RAM_MIRROR3_END = 0x61FFFFFF;

        // Memory access types
        enum class AccessType {
            READ,
            WRITE,
            EXECUTE
        };

        // Memory protection region
        struct ProtectedRegion {
            uint32_t start;
            uint32_t end;
            bool readOnly;
        };

        bool initMainRAM();
        void resetMainRAM();
        void shutdownMainRAM();

        // Basic memory access
        uint8_t read8(uint32_t address);
        uint16_t read16(uint32_t address);
        uint32_t read32(uint32_t address);
        uint64_t read64(uint32_t address);

        void write8(uint32_t address, uint8_t value);
        void write16(uint32_t address, uint16_t value);
        void write32(uint32_t address, uint32_t value);
        void write64(uint32_t address, uint64_t value);

        // Direct memory access
        uint8_t* getRawPointer(uint32_t address);
        bool isValidAddress(uint32_t address);
        uint32_t translateAddress(uint32_t address);

        // Memory protection
        void protectRegion(uint32_t start, uint32_t end, bool readOnly = false);
        void unprotectRegion(uint32_t start, uint32_t end);
        bool isRegionProtected(uint32_t address, AccessType type);

        // Bulk operations
        void memset(uint32_t address, uint8_t value, uint32_t size);
        void memcpy(uint32_t dest, uint32_t src, uint32_t size);

        // Debug and state management
        void saveState(std::ostream& stream);
        void loadState(std::istream& stream);
        std::string dumpMemory(uint32_t address, uint32_t size);
    }
}

#endif // SUPERPS2_MEM_MAINRAM_H 