#include "ee_core.h"
#include "../memory.h"
#include "../cop0/cop0.h"
#include <android/log.h>

#define TAG "EE_CPU"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

namespace ee {
namespace cpu {

// 128 registradores de 128 bits (nós começaremos com 32-bit para simplificar)
static uint32_t regs[32] = {0};
static uint32_t pc = 0x00000000;  // Program Counter

bool init() {
    reset();
    LOGI("EE CPU inicializada.");
    return true;
}

void shutdown() {
    LOGI("EE CPU finalizada.");
}

void reset() {
    for (int i = 0; i < 32; i++) regs[i] = 0;
    pc = 0x00000000;
    LOGI("EE CPU resetada.");
}

// Decodifica e executa uma instrução (simplificado)
void step() {
    uint32_t instr = memory::read32(pc);
    uint8_t opcode = (instr >> 26) & 0x3F;

    switch (opcode) {
        case 0x00: { // SPECIAL (ADD, SUB, etc)
            uint8_t funct = instr & 0x3F;
            uint8_t rd = (instr >> 11) & 0x1F;
            uint8_t rs = (instr >> 21) & 0x1F;
            uint8_t rt = (instr >> 16) & 0x1F;
            if (funct == 0x20) {  // ADD
                regs[rd] = regs[rs] + regs[rt];
            } else {
                LOGW("Instrução SPECIAL não implementada: funct=0x%02X", funct);
            }
            break;
        }

        case 0x08: { // ADDI
            uint8_t rt = (instr >> 16) & 0x1F;
            uint8_t rs = (instr >> 21) & 0x1F;
            int16_t imm = instr & 0xFFFF;
            regs[rt] = regs[rs] + imm;
            break;
        }

        case 0x0C: { // ANDI
            uint8_t rt = (instr >> 16) & 0x1F;
            uint8_t rs = (instr >> 21) & 0x1F;
            uint16_t imm = instr & 0xFFFF;
            regs[rt] = regs[rs] & imm;
            break;
        }

        case 0x3F: { // Fictício: HALT (só para debug)
            LOGI("Execução parada por HALT.");
            return;
        }

        default:
            LOGW("Opcode desconhecido: 0x%02X", opcode);
            break;
    }

    pc += 4;
}

// Loop contínuo da CPU (usado no modo interpretado)
void run() {
    for (int i = 0; i < 1000; ++i) {
        step();  // Executa 1000 instruções como simulação
    }
}

} // namespace cpu
} // namespace ee 