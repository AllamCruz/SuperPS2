#include "gpr.h"
#include <android/log.h>
#include <cstring>

#define TAG "GPR"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

namespace ee {
namespace cpu {

static std::array<uint64_t, GPR_COUNT> gpr = {};
uint32_t PC = 0;

bool initGPR() {
    std::fill(gpr.begin(), gpr.end(), 0);
    PC = 0;
    LOGI("Registradores GPR inicializados.");
    return true;
}

void resetGPR() {
    std::fill(gpr.begin(), gpr.end(), 0);
    PC = 0;
    LOGI("Registradores GPR resetados.");
}

void shutdownGPR() {
    LOGI("Registradores GPR liberados.");
}

void setGPR(uint8_t index, uint64_t value) {
    if (index == 0 || index >= GPR_COUNT) return; // $zero não pode ser alterado
    gpr[index] = value;
}

uint64_t getGPR(uint8_t index) {
    if (index >= GPR_COUNT) return 0;
    return gpr[index];
}

const std::array<uint64_t, GPR_COUNT>& getGPRArray() {
    return gpr;
}

} // namespace cpu
} // namespace ee 