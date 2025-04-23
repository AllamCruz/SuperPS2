#include "spu2.h"
#include <vector>
#include <cstring>
#include <android/log.h>

#define TAG "SuperPS2_SPU2"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// SPU2 possui 2MB de RAM
static constexpr size_t SPU2_RAM_SIZE = 2 * 1024 * 1024;
static std::vector<uint8_t> spu2Ram;

static bool initialized = false;

namespace spu2 {

bool init() {
    if (initialized) return true;
    spu2Ram.resize(SPU2_RAM_SIZE, 0);
    initialized = true;
    LOGI("SPU2 RAM alocada (2MB)");
    return true;
}

bool loadFirmware(const std::string& path) {
    // Stub opcional — PS2 não carrega firmware diretamente na SPU2
    LOGI("Firmware SPU2 não requerida diretamente.");
    return true;
}

void runFrame() {
    if (!initialized) return;
    // Emulação simplificada: aplicar mixagem ou enfileirar buffer
    LOGI("SPU2 executando frame de áudio.");
}

void writeRegister(uint32_t addr, uint16_t value) {
    // Registrar valores em MMIO simulados
    LOGI("SPU2 write: [%08X] <- %04X", addr, value);
}

uint16_t readRegister(uint32_t addr) {
    LOGI("SPU2 read: [%08X]", addr);
    return 0;
}

void reset() {
    if (!initialized) return;
    std::fill(spu2Ram.begin(), spu2Ram.end(), 0);
    LOGI("SPU2 resetado.");
}

void shutdown() {
    if (!initialized) return;
    spu2Ram.clear();
    initialized = false;
    LOGI("SPU2 desligado.");
}

} // namespace spu2 