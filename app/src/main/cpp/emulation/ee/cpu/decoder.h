#ifndef SUPERPS2_DECODER_H
#define SUPERPS2_DECODER_H

#include <cstdint>
#include <string>

namespace ee {
    namespace cpu {

        enum class InstructionType {
            UNKNOWN,
            ADDU,
            ADDIU,
            SUBU,
            OR,
            AND,
            XOR,
            NOR,
            LUI,
            LW,
            SW,
            J,
            JAL,
            JR,
            BEQ,
            BNE
        };

        struct DecodedInstruction {
            InstructionType type;
            uint8_t rs;
            uint8_t rt;
            uint8_t rd;
            uint16_t imm;
            uint32_t target;
            uint32_t raw;
        };

        DecodedInstruction decode(uint32_t instructionWord);
        std::string getInstructionName(InstructionType type);
    }
}

#endif // SUPERPS2_DECODER_H 