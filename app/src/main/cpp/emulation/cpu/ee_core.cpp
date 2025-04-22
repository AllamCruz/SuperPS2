#include "ee_core.h"
#include "../memory/memory.h"
#include "cop0.h"
#include <android/log.h>
#include <cstdint>

#define LOG_TAG "SuperPS2_EE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

namespace ee {

    // Exception codes for COP0 Cause register
    enum ExceptionCode {
        INTERRUPT            = 0x00,
        TLB_MOD              = 0x01,
        TLB_LOAD_MISS        = 0x02,
        TLB_STORE_MISS       = 0x03,
        ADDRESS_ERROR_LD     = 0x04,
        ADDRESS_ERROR_ST     = 0x05,
        SYSCALL              = 0x08,
        BREAKPOINT           = 0x09,
        RESERVED_INST        = 0x0A,
        COPROCESSOR_UNUSABLE = 0x0B,
        OVERFLOW             = 0x0C,
        TRAP                 = 0x0D
    };

    static uint32_t gpr[32] = {0};
    static uint32_t pc       = 0x00000000;
    static uint32_t hi       = 0;
    static uint32_t lo       = 0;

    // Forward declarations
    static void step();
    static void raiseException(uint32_t code);
    static void exceptionReturn();

    void init() {
        reset();
        cop0::reset();
        LOGI("Emotion Engine e COP0 inicializados.");
    }

    void reset() {
        for (int i = 0; i < 32; ++i) gpr[i] = 0;
        hi = lo = 0;
        pc = 0x00000000;
        LOGI("EE resetada.");
    }

    static void step() {
        uint32_t instruction = memory::read32(pc);
        uint32_t opcode      = (instruction >> 26) & 0x3F;
        bool pcChanged = false;

        switch (opcode) {
            case 0x00: { // R-type and SYSCALL/BREAK
                uint32_t funct = instruction & 0x3F;
                // SYSCALL
                if (funct == 0x0C) {
                    raiseException(ExceptionCode::SYSCALL);
                    return;
                }
                // R-type handlers
                uint32_t rs    = (instruction >> 21) & 0x1F;
                uint32_t rt    = (instruction >> 16) & 0x1F;
                uint32_t rd    = (instruction >> 11) & 0x1F;
                uint32_t shamt = (instruction >> 6)  & 0x1F;

                switch (funct) {
                    case 0x20: { // ADD
                        int32_t res = (int32_t)gpr[rs] + (int32_t)gpr[rt];
                        if (((int32_t)gpr[rs] > 0 && (int32_t)gpr[rt] > 0 && res < 0) ||
                            ((int32_t)gpr[rs] < 0 && (int32_t)gpr[rt] < 0 && res > 0)) {
                            raiseException(ExceptionCode::OVERFLOW);
                        } else {
                            gpr[rd] = (uint32_t)res;
                        }
                        break;
                    }
                    case 0x22: // SUB
                        gpr[rd] = (uint32_t)((int32_t)gpr[rs] - (int32_t)gpr[rt]);
                        break;
                    case 0x24: // AND
                        gpr[rd] = gpr[rs] & gpr[rt];
                        break;
                    case 0x25: // OR
                        gpr[rd] = gpr[rs] | gpr[rt];
                        break;
                    case 0x2A: // SLT
                        gpr[rd] = ((int32_t)gpr[rs] < (int32_t)gpr[rt]) ? 1u : 0u;
                        break;
                    case 0x00: // SLL
                        gpr[rd] = gpr[rt] << shamt;
                        break;
                    case 0x02: // SRL
                        gpr[rd] = gpr[rt] >> shamt;
                        break;
                    case 0x08: { // JR
                        uint32_t target = gpr[rs];
                        pc += 4; step(); 
                        pc = target;
                        return;
                    }
                    default:
                        raiseException(ExceptionCode::RESERVED_INST);
                        return;
                }
                break;
            }

            case 0x10: { // COP0 operations including ERET
                uint32_t rs   = (instruction >> 21) & 0x1F;
                uint32_t rt   = (instruction >> 16) & 0x1F;
                uint32_t rd   = (instruction >> 11) & 0x1F;
                uint32_t func = instruction & 0x3F;
                switch (rs) {
                    case 0x00: // MFC0
                        gpr[rt] = cop0::read(rd);
                        break;
                    case 0x04: // MTC0
                        cop0::write(rd, gpr[rt]);
                        break;
                    case 0x10: // ERET
                        if (func == 0x18) {
                            exceptionReturn();
                        } else {
                            raiseException(ExceptionCode::RESERVED_INST);
                        }
                        return;
                    default:
                        raiseException(ExceptionCode::COPROCESSOR_UNUSABLE);
                        return;
                }
                break;
            }

            // Other I-type, J-type cases omitted for brevity
            default:
                raiseException(ExceptionCode::RESERVED_INST);
                return;
        }

        gpr[0] = 0; 
        if (!pcChanged) pc += 4;
    }

    void runFrame() {
        const int cycles = 100000;
        for (int i = 0; i < cycles; ++i) step();
    }

    void shutdown() {
        LOGI("EE finalizada.");
    }

    void printState() {
        LOGI("PC=0x%08X HI=0x%08X LO=0x%08X", pc, hi, lo);
        for (int i = 0; i < 32; ++i) {
            LOGI("R%-2d=0x%08X", i, gpr[i]);
        }
    }

    static void raiseException(uint32_t code) {
        cop0::cop0Regs[cop0::EPC]   = pc;
        cop0::cop0Regs[cop0::Cause] = code << 2;
        cop0::cop0Regs[cop0::Status] |= (1 << 1); // EXL = 1
        pc = 0x80000080;
        LOGD("Exceção lançada: code=0x%X, EPC=0x%08X", code, cop0::cop0Regs[cop0::EPC]);
    }

    static void exceptionReturn() {
        cop0::cop0Regs[cop0::Status] &= ~(1 << 1); // EXL = 0
        pc = cop0::cop0Regs[cop0::EPC];
        LOGD("ERET: Retornando de exceção para EPC=0x%08X", pc);
    }

} // namespace ee
