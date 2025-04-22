#include "memory.h"
#include <vector>
#include <cstring>
#include <android/log.h>

#define TAG "EE_MEMORY"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// Tamanho da RAM principal do EE: 32 MB
static constexpr size_t EE_RAM_SIZE = 32 * 1024 * 1024;
static std::vector<uint8_t> eeRam;

namespace ee {
namespace memory {

bool init() {
    eeRam.resize(EE_RAM_SIZE, 0);
    LOGI("EE RAM inicializada (32MB)");
    return true;
}

void shutdown() {
    eeRam.clear();
    LOGI("EE RAM liberada.");
}

void reset() {
    std::fill(eeRam.begin(), eeRam.end(), 0);
    LOGI("EE RAM resetada.");
}

// Leitura de memória
uint8_t read8(uint32_t addr) {
    if (addr < EE_RAM_SIZE) return eeRam[addr];
    LOGW("Leitura 8-bit fora da RAM: %08X", addr);
    return 0;
}

uint16_t read16(uint32_t addr) {
    if (addr + 1 < EE_RAM_SIZE) {
        return static_cast<uint16_t>(eeRam[addr]) |
               (static_cast<uint16_t>(eeRam[addr + 1]) << 8);
    }
    LOGW("Leitura 16-bit fora da RAM: %08X", addr);
    return 0;
}

uint32_t read32(uint32_t addr) {
    if (addr + 3 < EE_RAM_SIZE) {
        return static_cast<uint32_t>(eeRam[addr]) |
               (static_cast<uint32_t>(eeRam[addr + 1]) << 8) |
               (static_cast<uint32_t>(eeRam[addr + 2]) << 16) |
               (static_cast<uint32_t>(eeRam[addr + 3]) << 24);
    }
    LOGW("Leitura 32-bit fora da RAM: %08X", addr);
    return 0;
}

uint64_t read64(uint32_t addr) {
    return static_cast<uint64_t>(read32(addr)) |
           (static_cast<uint64_t>(read32(addr + 4)) << 32);
}

// Escrita de memória
void write8(uint32_t addr, uint8_t value) {
    if (addr < EE_RAM_SIZE) {
        eeRam[addr] = value;
    } else {
        LOGW("Escrita 8-bit fora da RAM: %08X", addr);
    }
}

void write16(uint32_t addr, uint16_t value) {
    if (addr + 1 < EE_RAM_SIZE) {
        eeRam[addr] = value & 0xFF;
        eeRam[addr + 1] = (value >> 8) & 0xFF;
    } else {
        LOGW("Escrita 16-bit fora da RAM: %08X", addr);
    }
}

void write32(uint32_t addr, uint32_t value) {
    if (addr + 3 < EE_RAM_SIZE) {
        eeRam[addr] = value & 0xFF;
        eeRam[addr + 1] = (value >> 8) & 0xFF;
        eeRam[addr + 2] = (value >> 16) & 0xFF;
        eeRam[addr + 3] = (value >> 24) & 0xFF;
    } else {
        LOGW("Escrita 32-bit fora da RAM: %08X", addr);
    }
}

void write64(uint32_t addr, uint64_t value) {
    write32(addr, static_cast<uint32_t>(value & 0xFFFFFFFF));
    write32(addr + 4, static_cast<uint32_t>((value >> 32) & 0xFFFFFFFF));
}

} // namespace memory
} // namespace ee 