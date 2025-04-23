#pragma once

#include <stdint.h>

namespace ee {
    namespace cpu {
        void execInstruction(uint32_t instructionWord);
    }
}