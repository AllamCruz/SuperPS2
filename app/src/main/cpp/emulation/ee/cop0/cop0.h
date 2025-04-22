#ifndef SUPERPS2_COP0_H
#define SUPERPS2_COP0_H

#include <cstdint>

namespace ee {
    namespace cop0 {
        bool init();
        void reset();
        void shutdown();

        uint32_t readRegister(uint8_t reg);
        void writeRegister(uint8_t reg, uint32_t value);

        void handleException(uint32_t cause);
        void returnFromException();
    }
}

#endif // SUPERPS2_COP0_H 