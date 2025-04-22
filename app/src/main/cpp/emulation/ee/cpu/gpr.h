#ifndef SUPERPS2_GPR_H
#define SUPERPS2_GPR_H

#include <cstdint>
#include <string>
#include <array>

namespace ee {
    namespace cpu {

        constexpr int GPR_COUNT = 32;

        // Special register indices
        constexpr int ZERO_REG = 0;  // Always zero
        constexpr int RA_REG = 31;   // Return address
        constexpr int SP_REG = 29;   // Stack pointer
        constexpr int FP_REG = 30;   // Frame pointer

        bool initGPR();
        void resetGPR();
        void shutdownGPR();

        // Basic register access
        uint64_t getGPR(uint8_t index);
        void setGPR(uint8_t index, uint64_t value);

        // Special register access
        uint64_t getRA();
        void setRA(uint64_t value);
        uint64_t getSP();
        void setSP(uint64_t value);
        uint64_t getFP();
        void setFP(uint64_t value);

        // Register pair access (for 128-bit operations)
        void getGPRPair(int index, uint64_t& hi, uint64_t& lo);
        void setGPRPair(int index, uint64_t hi, uint64_t lo);

        // Debugging and state management
        std::string dumpGPR();
        void saveState(std::ostream& stream);
        void loadState(std::istream& stream);

        const std::array<uint64_t, GPR_COUNT>& getGPRArray();

        // Program Counter
        extern uint32_t PC;
    }
}

#endif // SUPERPS2_GPR_H 