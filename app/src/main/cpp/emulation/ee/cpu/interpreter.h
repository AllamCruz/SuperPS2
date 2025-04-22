#ifndef SUPERPS2_INTERPRETER_H
#define SUPERPS2_INTERPRETER_H

#include <cstdint>

namespace ee {
    namespace cpu {
        void execInstruction(uint32_t instructionWord);
    }
}

#endif // SUPERPS2_INTERPRETER_H 