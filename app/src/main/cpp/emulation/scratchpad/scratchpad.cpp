#include "scratchpad.h"
#include <cstdlib>
#include <cstring>
#include <android/log.h>

#define TAG "Scratchpad"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// PS2 scratchpad base e tamanho
static constexpr uint32_t BASE_ADDR = 0x1F800000;
static constexpr size_t   SIZE      = 8 * 1024; // 8 KB

static uint8_t* buffer = nullptr;

namespace scratchpad {

bool init() {
    if (buffer) return true;
    buffer = static_cast<uint8_t*>(malloc(SIZE));
    if (!buffer) {
        LOGE("Falha ao alocar Scratchpad");
        return false;
    }
    memset(buffer, 0, SIZE);
    LOGI("Scratchpad de 8KB inicializado");
    return true;
}

void shutdown() {
    if (buffer) {
        free(buffer);
        buffer = nullptr;
        LOGI("Scratchpad liberado");
    }
}

bool isScratchpadAddr(uint32_t addr) {
    return addr >= BASE_ADDR && addr < BASE_ADDR + SIZE;
}

uint32_t addrToOffset(uint32_t addr) {
    return addr - BASE_ADDR;
}

uint8_t read8(uint32_t offset) {
    if (offset >= SIZE) {
        LOGE("Leitura scratchpad 8-bit fora de alcance: 0x%X", offset);
        return 0;
    }
    return buffer[offset];
}

uint16_t read16(uint32_t offset) {
    if (offset + 1 >= SIZE) {
        LOGE("Leitura scratchpad 16-bit fora de alcance: 0x%X", offset);
        return 0;
    }
    return *reinterpret_cast<uint16_t*>(buffer + offset);
}

uint32_t read32(uint32_t offset) {
    if (offset + 3 >= SIZE) {
        LOGE("Leitura scratchpad 32-bit fora de alcance: 0x%X", offset);
        return 0;
    }
    return *reinterpret_cast<uint32_t*>(buffer + offset);
}

void write8(uint32_t offset, uint8_t value) {
    if (offset >= SIZE) {
        LOGE("Escrita scratchpad 8-bit fora de alcance: 0x%X", offset);
        return;
    }
    buffer[offset] = value;
}

void write16(uint32_t offset, uint16_t value) {
    if (offset + 1 >= SIZE) {
        LOGE("Escrita scratchpad 16-bit fora de alcance: 0x%X", offset);
        return;
    }
    *reinterpret_cast<uint16_t*>(buffer + offset) = value;
}

void write32(uint32_t offset, uint32_t value) {
    if (offset + 3 >= SIZE) {
        LOGE("Escrita scratchpad 32-bit fora de alcance: 0x%X", offset);
        return;
    }
    *reinterpret_cast<uint32_t*>(buffer + offset) = value;
}

} // namespace scratchpad 