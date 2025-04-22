#include "vu0.h"
#include <array>
#include <android/log.h>

#define TAG "VU0"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// Registradores vetoriais: 32 vetores de 4 floats (x, y, z, w)
static std::array<std::array<float, 4>, 32> vectorRegs;
// Registradores escalares
static std::array<float, 16> scalarRegs;

namespace ee {
namespace vu0 {

bool init() {
    reset();
    LOGI("VU0 inicializado.");
    return true;
}

void reset() {
    for (auto& vec : vectorRegs)
        vec = {0.0f, 0.0f, 0.0f, 0.0f};
    scalarRegs.fill(0.0f);
    LOGI("VU0 resetado.");
}

void shutdown() {
    LOGI("VU0 desligado.");
}

// Simula execução de instrução vetorial (ex: ADD.vf)
void executeCycle() {
    // Exemplo: ADD vf1 = vf2 + vf3
    for (int i = 0; i < 4; i++) {
        vectorRegs[1][i] = vectorRegs[2][i] + vectorRegs[3][i];
    }
    LOGI("VU0: vf1 = vf2 + vf3 executado.");
}

void run() {
    for (int i = 0; i < 1000; i++) {
        executeCycle(); // Simula instruções vetoriais
    }
}

void setRegister(uint8_t reg, float value) {
    if (reg >= 16) return;
    scalarRegs[reg] = value;
}

float getRegister(uint8_t reg) {
    if (reg >= 16) return 0.0f;
    return scalarRegs[reg];
}

} // namespace vu0
} // namespace ee 