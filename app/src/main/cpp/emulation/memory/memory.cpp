#include "memory.h"
#include <cstdlib>
#include <cstring>
#include <android/log.h>

#define LOG_TAG "SuperPS2Memory"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace memory {

    static const size_t RAM_SIZE = 0x02000000; // 32MB
    static uint8_t* mainRAM = nullptr;

    bool init() {
        if (mainRAM) return true; // já está inicializado
        mainRAM = static_cast<uint8_t*>(malloc(RAM_SIZE));
        if (!mainRAM) {
            LOGE("Falha ao alocar RAM principal.");
            return false;
        }
        memset(mainRAM, 0, RAM_SIZE);
        LOGI("Memória principal de 32MB alocada com sucesso.");
        return true;
    }

    void shutdown() {
        if (mainRAM) {
            free(mainRAM);
            mainRAM = nullptr;
            LOGI("Memória principal liberada.");
        }
    }

    bool isValidAddress(uint32_t addr) {
        return addr < RAM_SIZE;
    }

    uint8_t read8(uint32_t addr) {
        if (!isValidAddress(addr)) {
            LOGE("Leitura inválida (8 bits) no endereço: 0x%08X", addr);
            return 0;
        }
        return mainRAM[addr];
    }

    uint16_t read16(uint32_t addr) {
        if (!isValidAddress(addr + 1)) {
            LOGE("Leitura inválida (16 bits) no endereço: 0x%08X", addr);
            return 0;
        }
        return *reinterpret_cast<uint16_t*>(&mainRAM[addr]);
    }

    uint32_t read32(uint32_t addr) {
        if (!isValidAddress(addr + 3)) {
            LOGE("Leitura inválida (32 bits) no endereço: 0x%08X", addr);
            return 0;
        }
        return *reinterpret_cast<uint32_t*>(&mainRAM[addr]);
    }

    void write8(uint32_t addr, uint8_t value) {
        if (!isValidAddress(addr)) {
            LOGE("Escrita inválida (8 bits) no endereço: 0x%08X", addr);
            return;
        }
        mainRAM[addr] = value;
    }

    void write16(uint32_t addr, uint16_t value) {
        if (!isValidAddress(addr + 1)) {
            LOGE("Escrita inválida (16 bits) no endereço: 0x%08X", addr);
            return;
        }
        *reinterpret_cast<uint16_t*>(&mainRAM[addr]) = value;
    }

    void write32(uint32_t addr, uint32_t value) {
        if (!isValidAddress(addr + 3)) {
            LOGE("Escrita inválida (32 bits) no endereço: 0x%08X", addr);
            return;
        }
        *reinterpret_cast<uint32_t*>(&mainRAM[addr]) = value;
    }
}
