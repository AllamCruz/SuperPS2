#include "decoder.h"
#include <android/log.h>

#define TAG "Decoder"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

namespace ee {
namespace cpu {

DecodedInstruction decode(uint32_t instr) {
    DecodedInstruction di{};
    di.raw = instr;

    uint8_t opcode = (instr >> 26) & 0x3F;
    di.rs = (instr >> 21) & 0x1F;
    di.rt = (instr >> 16) & 0x1F;
    di.rd = (instr >> 11) & 0x1F;
    di.imm = instr & 0xFFFF;
    di.target = instr & 0x3FFFFFF;

    switch (opcode) {
        case 0x00: { // R-Type
            uint8_t funct = instr & 0x3F;
            switch (funct) {
                case 0x21: di.type = InstructionType::ADDU; break;
                case 0x23: di.type = InstructionType::SUBU; break;
                case 0x25: di.type = InstructionType::OR; break;
                case 0x24: di.type = InstructionType::AND; break;
                case 0x26: di.type = InstructionType::XOR; break;
                case 0x27: di.type = InstructionType::NOR; break;
                case 0x08: di.type = InstructionType::JR; break;
                default: di.type = InstructionType::UNKNOWN; break;
            }
            break;
        }
        case 0x09: di.type = InstructionType::ADDIU; break;
        case 0x0F: di.type = InstructionType::LUI; break;
        case 0x23: di.type = InstructionType::LW; break;
        case 0x2B: di.type = InstructionType::SW; break;
        case 0x02: di.type = InstructionType::J; break;
        case 0x03: di.type = InstructionType::JAL; break;
        case 0x04: di.type = InstructionType::BEQ; break;
        case 0x05: di.type = InstructionType::BNE; break;
        default: di.type = InstructionType::UNKNOWN; break;
    }

    return di;
}

std::string getInstructionName(InstructionType type) {
    switch (type) {
        case InstructionType::ADDU: return "ADDU";
        case InstructionType::ADDIU: return "ADDIU";
        case InstructionType::SUBU: return "SUBU";
        case InstructionType::OR: return "OR";
        case InstructionType::AND: return "AND";
        case InstructionType::XOR: return "XOR";
        case InstructionType::NOR: return "NOR";
        case InstructionType::LUI: return "LUI";
        case InstructionType::LW: return "LW";
        case InstructionType::SW: return "SW";
        case InstructionType::J: return "J";
        case InstructionType::JAL: return "JAL";
        case InstructionType::JR: return "JR";
        case InstructionType::BEQ: return "BEQ";
        case InstructionType::BNE: return "BNE";
        default: return "UNKNOWN";
    }
}

} // namespace cpu
} // namespace ee 