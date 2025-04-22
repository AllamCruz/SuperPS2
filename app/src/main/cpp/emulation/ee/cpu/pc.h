#ifndef SUPERPS2_PC_H
#define SUPERPS2_PC_H

#include <cstdint>

namespace ee {
    namespace cpu {

        constexpr uint32_t PC_ALIGNMENT = 4;  // MIPS instructions are 4 bytes aligned
        constexpr uint32_t PC_INITIAL = 0x00000000;
        constexpr uint32_t PC_BIOS_START = 0x1FC00000;  // BIOS start address

        bool initPC();
        void resetPC();
        void shutdownPC();

        // Basic PC operations
        void setPC(uint32_t value);
        uint32_t getPC();
        void advancePC();

        // Branch and jump operations
        void branchPC(int32_t offset);  // Relative branch
        void jumpPC(uint32_t target);   // Absolute jump
        void jumpRegister(uint32_t address);  // Jump to register value

        // Exception handling
        void setExceptionPC(uint32_t handler);
        uint32_t getExceptionPC();

        // Debug and state management
        void saveState(std::ostream& stream);
        void loadState(std::istream& stream);
    }
}

#endif // SUPERPS2_PC_H 