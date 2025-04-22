#include "interpreter.h"
#include "decoder.h"
#include "gpr.h"
#include "../memory/memory.h"
#include <android/log.h>

#define TAG "Interpreter"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

namespace ee {
namespace cpu {

void execInstruction(uint32_t instructionWord) {
    auto instr = decode(instructionWord);

    switch (instr.type) {
        case InstructionType::ADDU: {
            uint64_t v1 = getGPR(instr.rs);
            uint64_t v2 = getGPR(instr.rt);
            setGPR(instr.rd, v1 + v2);
            LOGI("ADDU R%d = R%d + R%d", instr.rd, instr.rs, instr.rt);
            break;
        }
        case InstructionType::ADDIU: {
            uint64_t v = getGPR(instr.rs);
            setGPR(instr.rt, v + (int16_t)instr.imm);
            LOGI("ADDIU R%d = R%d + %d", instr.rt, instr.rs, (int16_t)instr.imm);
            break;
        }
        case InstructionType::SUBU: {
            uint64_t v1 = getGPR(instr.rs);
            uint64_t v2 = getGPR(instr.rt);
            setGPR(instr.rd, v1 - v2);
            LOGI("SUBU R%d = R%d - R%d", instr.rd, instr.rs, instr.rt);
            break;
        }
        case InstructionType::OR: {
            setGPR(instr.rd, getGPR(instr.rs) | getGPR(instr.rt));
            break;
        }
        case InstructionType::AND: {
            setGPR(instr.rd, getGPR(instr.rs) & getGPR(instr.rt));
            break;
        }
        case InstructionType::XOR: {
            setGPR(instr.rd, getGPR(instr.rs) ^ getGPR(instr.rt));
            break;
        }
        case InstructionType::NOR: {
            setGPR(instr.rd, ~(getGPR(instr.rs) | getGPR(instr.rt)));
            break;
        }
        case InstructionType::LUI: {
            setGPR(instr.rt, (uint64_t)instr.imm << 16);
            break;
        }
        case InstructionType::JR: {
            PC = static_cast<uint32_t>(getGPR(instr.rs));
            return; // não incrementa o PC
        }
        case InstructionType::J: {
            PC = (PC & 0xF0000000) | (instr.target << 2);
            return;
        }
        case InstructionType::JAL: {
            setGPR(31, PC + 4);
            PC = (PC & 0xF0000000) | (instr.target << 2);
            return;
        }
        case InstructionType::BEQ:
        case InstructionType::BNE:
        case InstructionType::LW: {
            uint32_t addr = getGPR(instr.rs) + (int16_t)instr.imm;
            uint32_t value = ee::memory::read32(addr);
            setGPR(instr.rt, value);
            break;
        }
        case InstructionType::SW: {
            uint32_t addr = getGPR(instr.rs) + (int16_t)instr.imm;
            ee::memory::write32(addr, static_cast<uint32_t>(getGPR(instr.rt)));
            break;
        }
        case InstructionType::UNKNOWN:
        default:
            LOGI("Instrução desconhecida ou não implementada: 0x%08X", instructionWord);
            break;
    }
}

} // namespace cpu
} // namespace ee 