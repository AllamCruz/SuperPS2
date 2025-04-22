#include "cop0.h"
#include <array>
#include <android/log.h>

#define TAG "EE_COP0"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// COP0 tem 32 registradores de 32 bits
static std::array<uint32_t, 32> cop0Registers;

namespace ee {
namespace cop0 {

bool init() {
    reset();
    LOGI("COP0 inicializado.");
    return true;
}

void reset() {
    cop0Registers.fill(0);
    LOGI("COP0 resetado.");
}

void shutdown() {
    LOGI("COP0 finalizado.");
}

uint32_t readRegister(uint8_t reg) {
    if (reg >= 32) {
        LOGW("Leitura de registrador inválido do COP0: %d", reg);
        return 0;
    }
    return cop0Registers[reg];
}

void writeRegister(uint8_t reg, uint32_t value) {
    if (reg >= 32) {
        LOGW("Escrita em registrador inválido do COP0: %d", reg);
        return;
    }
    cop0Registers[reg] = value;
}

void handleException(uint32_t cause) {
    // Exemplo básico de exceção
    cop0Registers[13] = cause; // Cause
    cop0Registers[14] = 0xDEADBEEF; // EPC fictício (normalmente PC anterior)
    cop0Registers[12] |= 0x2; // Status: entra em modo exceção

    LOGI("Exceção tratada: cause=%08X", cause);
}

void returnFromException() {
    cop0Registers[12] &= ~0x2; // Sai do modo exceção
    LOGI("Retornando de exceção para EPC=%08X", cop0Registers[14]);
}

} // namespace cop0
} // namespace ee 