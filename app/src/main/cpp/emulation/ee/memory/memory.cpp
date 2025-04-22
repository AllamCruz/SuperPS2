#include "memory.h"
#include <cstdlib>
#include <cstring>
#include <android/log.h>

#define TAG "EEMemory"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

namespace ee {
namespace memory {

// PS2 tem 32MB de RAM
constexpr size_t RAM_SIZE = 32 * 1024 * 1024;
uint8_t* ps2Ram = nullptr;

void initMemory() {
    ps2Ram = static_cast<uint8_t*>(malloc(RAM_SIZE));
    if (!ps2Ram) {
        LOGI("Falha ao alocar memória RAM do PS2.");
        exit(1);
    }
    memset(ps2Ram, 0, RAM_SIZE);
    LOGI("Memória RAM do PS2 inicializada (%zu bytes).", RAM_SIZE);
}

void shutdownMemory() {
    if (ps2Ram) {
        free(ps2Ram);
        ps2Ram = nullptr;
        LOGI("Memória RAM liberada.");
    }
}

uint32_t read32(uint32_t addr) {
    if (addr < RAM_SIZE - 3) {
        uint32_t value = *(uint32_t*)(ps2Ram + addr);
        LOGI("read32: [0x%08X] => 0x%08X", addr, value);
        return value;
    }
    LOGI("Tentativa de leitura inválida: 0x%08X", addr);
    return 0;
}

void write32(uint32_t addr, uint32_t value) {
    if (addr < RAM_SIZE - 3) {
        *(uint32_t*)(ps2Ram + addr) = value;
        LOGI("write32: [0x%08X] <= 0x%08X", addr, value);
    } else {
        LOGI("Tentativa de escrita inválida: 0x%08X", addr);
    }
}

uint8_t* getPointer(uint32_t addr) {
    if (addr < RAM_SIZE) {
        return ps2Ram + addr;
    }
    return nullptr;
}

} // namespace memory
} // namespace ee 