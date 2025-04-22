#include "dmac.h"
#include <android/log.h>
#include <array>

#define TAG "DMAC"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

namespace ee {
namespace dma {

struct DMATransfer {
    uint32_t srcAddr;
    uint32_t dstAddr;
    uint32_t size;
    bool active;
};

static std::array<DMATransfer, static_cast<size_t>(Channel::COUNT)> dmaChannels;

bool init() {
    reset();
    LOGI("DMA Controller iniciado.");
    return true;
}

void reset() {
    for (auto& ch : dmaChannels) {
        ch = {0, 0, 0, false};
    }
    LOGI("DMA Controller resetado.");
}

void shutdown() {
    LOGI("DMA Controller desligado.");
}

void startTransfer(Channel channel, uint32_t srcAddr, uint32_t dstAddr, uint32_t size) {
    size_t index = static_cast<size_t>(channel);
    dmaChannels[index] = {srcAddr, dstAddr, size, true};
    LOGI("DMA iniciado no canal %zu: 0x%X -> 0x%X, %u bytes", index, srcAddr, dstAddr, size);
}

void update() {
    for (auto& ch : dmaChannels) {
        if (ch.active && ch.size > 0) {
            // Simular transferência
            ch.size -= 128; // Simula 128 bytes por ciclo
            if (ch.size <= 0) {
                ch.active = false;
                LOGI("DMA canal concluído.");
            }
        }
    }
}

void simulateCycle() {
    update();
}

bool isTransferComplete(Channel channel) {
    return !dmaChannels[static_cast<size_t>(channel)].active;
}

} // namespace dma
} // namespace ee 