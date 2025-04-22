#include "iop.h"
#include "../memory/memory.h"
#include <android/log.h>
#include <vector>

#define TAG "SuperPS2_IOP"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// IOP has 2 MB RAM at 0x1FC00000
static constexpr uint32_t IOP_RAM_BASE = 0x1FC00000;
static constexpr size_t   IOP_RAM_SIZE = 2 * 1024 * 1024;

static std::vector<uint8_t> iopRam;
static bool initialized = false;

namespace iop {

bool init() {
    if (initialized) return true;
    iopRam.assign(IOP_RAM_SIZE, 0);
    LOGI("IOP RAM (2 MB) allocated");
    initialized = true;
    return true;
}

bool loadBios(const std::string& path) {
    // stub: carregar imagem BIOS do IOP
    LOGI("IOP BIOS load requested: %s", path.c_str());
    return initialized;
}

void runFrame() {
    if (!initialized) return;
    // stub: interpretar instruções do IOP aqui
    LOGI("IOP runFrame() stub executed.");
    // após processar, notificar possíveis DMAs
}

void handleDmaInterrupt(int channel) {
    // stub: responder a transferências de canais CDVD, SPU2, etc.
    LOGI("IOP DMA interrupt: channel %d", channel);
}

void reset() {
    if (!initialized) return;
    std::fill(iopRam.begin(), iopRam.end(), 0);
    LOGI("IOP reset.");
}

void shutdown() {
    if (!initialized) return;
    iopRam.clear();
    initialized = false;
    LOGI("IOP shutdown.");
}

} // namespace iop 