#include "ee_core.h"
#include "../memory/memory.h"
#include <android/log.h>

#define LOG_TAG "SuperPS2_EE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace ee {

    static uint32_t gpr[32] = {0};  // Registradores gerais
    static uint32_t pc = 0x00000000; // Program Counter
    static uint32_t hi = 0;
    static uint32_t lo = 0;

    void init() {
        reset();
        LOGI("Emotion Engine inicializada.");
    }

    void reset() {
        for (int i = 0; i < 32; i++) gpr[i] = 0;
        hi = lo = 0;
        pc = 0x00000000; // Será definido após o carregamento da BIOS ou ELF
        LOGI("EE resetada.");
    }

    static void step() {
        uint32_t instruction = memory::read32(pc);
        uint32_t opcode = (instruction >> 26) & 0x3F;
        bool pcChanged = false;

        switch (opcode) {
            case 0x00: {
                uint32_t rs    = (instruction >> 21) & 0x1F;
                uint32_t rt    = (instruction >> 16) & 0x1F;
                uint32_t rd    = (instruction >> 11) & 0x1F;
                uint32_t shamt = (instruction >> 6) & 0x1F;
                uint32_t funct = instruction & 0x3F;

                switch (funct) {
                    case 0x20: {
                        int32_t result = (int32_t)gpr[rs] + (int32_t)gpr[rt];
                        if (((int32_t)gpr[rs] > 0 && (int32_t)gpr[rt] > 0 && result < 0) ||
                            ((int32_t)gpr[rs] < 0 && (int32_t)gpr[rt] < 0 && result > 0)) {
                            LOGE("Overflow em ADD no PC 0x%08X", pc);
                            break;
                        }
                        gpr[rd] = result;
                        break;
                    }
                    case 0x22:
                        gpr[rd] = (int32_t)gpr[rs] - (int32_t)gpr[rt];
                        break;
                    case 0x24:
                        gpr[rd] = gpr[rs] & gpr[rt];
                        break;
                    case 0x25:
                        gpr[rd] = gpr[rs] | gpr[rt];
                        break;
                    case 0x2A:
                        gpr[rd] = ((int32_t)gpr[rs] < (int32_t)gpr[rt]) ? 1 : 0;
                        break;
                    case 0x00:
                        gpr[rd] = gpr[rt] << shamt;
                        break;
                    case 0x02:
                        gpr[rd] = gpr[rt] >> shamt;
                        break;
                    default:
                        LOGE("Funct desconhecido: 0x%02X no PC: 0x%08X", funct, pc);
                        break;
                }
                break;
            }

            case 0x08: {
                int32_t imm = (int16_t)(instruction & 0xFFFF);
                uint32_t rs = (instruction >> 21) & 0x1F;
                uint32_t rt = (instruction >> 16) & 0x1F;
                int32_t result = (int32_t)gpr[rs] + imm;

                if (((int32_t)gpr[rs] > 0 && imm > 0 && result < 0) ||
                    ((int32_t)gpr[rs] < 0 && imm < 0 && result > 0)) {
                    LOGE("Overflow em ADDI no PC: 0x%08X", pc);
                    break;
                }
                gpr[rt] = result;
                break;
            }
            case 0x0C: {
                uint16_t imm = instruction & 0xFFFF;
                uint32_t rs = (instruction >> 21) & 0x1F;
                uint32_t rt = (instruction >> 16) & 0x1F;
                gpr[rt] = gpr[rs] & imm;
                break;
            }
            case 0x0D: {
                uint16_t imm = instruction & 0xFFFF;
                uint32_t rs = (instruction >> 21) & 0x1F;
                uint32_t rt = (instruction >> 16) & 0x1F;
                gpr[rt] = gpr[rs] | imm;
                break;
            }
            case 0x23: {
                int16_t imm = instruction & 0xFFFF;
                uint32_t rs = (instruction >> 21) & 0x1F;
                uint32_t rt = (instruction >> 16) & 0x1F;
                uint32_t addr = gpr[rs] + imm;
                gpr[rt] = memory::read32(addr);
                break;
            }
            case 0x2B: {
                int16_t imm = instruction & 0xFFFF;
                uint32_t rs = (instruction >> 21) & 0x1F;
                uint32_t rt = (instruction >> 16) & 0x1F;
                uint32_t addr = gpr[rs] + imm;
                memory::write32(addr, gpr[rt]);
                break;
            }
            case 0x04: {
                uint32_t rs = (instruction >> 21) & 0x1F;
                uint32_t rt = (instruction >> 16) & 0x1F;
                int16_t offset = (int16_t)(instruction & 0xFFFF);
                if (gpr[rs] == gpr[rt]) {
                    pc += (offset << 2);
                    pcChanged = true;
                }
                break;
            }
            case 0x05: {
                uint32_t rs = (instruction >> 21) & 0x1F;
                uint32_t rt = (instruction >> 16) & 0x1F;
                int16_t offset = (int16_t)(instruction & 0xFFFF);
                if (gpr[rs] != gpr[rt]) {
                    pc += (offset << 2);
                    pcChanged = true;
                }
                break;
            }
            default:
                LOGE("Opcode desconhecido: 0x%02X no PC: 0x%08X", opcode, pc);
                break;
        }

        gpr[0] = 0;
        if (!pcChanged) pc += 4;
    }

    void runFrame() {
        const int cyclesPerFrame = 100000;
        for (int i = 0; i < cyclesPerFrame; i++) {
            step();
        }
    }

    void shutdown() {
        LOGI("EE finalizada.");
    }

    void printState() {
        LOGI("PC: 0x%08X | HI: 0x%08X | LO: 0x%08X", pc, hi, lo);
        for (int i = 0; i < 32; i++) {
            LOGI("R%d: 0x%08X", i, gpr[i]);
        }
    }

}