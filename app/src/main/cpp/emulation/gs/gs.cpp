#include "gs.h"
#include "gs_pipeline.h"
#include "gs_packet_decoder.h"
#include <android/log.h>
#include <cstring>

#define TAG "GS"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// Exemplo de framebuffer em memória: RGBA8888
static uint8_t* frameBuffer = nullptr;
static int fbWidth = 0, fbHeight = 0;

namespace gs {

static bool initialized = false;

bool init() {
    if (initialized) {
        LOGI("GS já inicializado");
        return true;
    }

    if (!g_pipeline.init()) {
        LOGI("Falha ao inicializar pipeline GS");
        return false;
    }

    initialized = true;
    LOGI("GS inicializado com sucesso");
    return true;
}

void writeRegister(uint32_t reg, uint64_t data) {
    if (!initialized) {
        LOGI("GS não inicializado");
        return;
    }

    g_pipeline.writeRegister(reg, data);
}

void reset() {
    if (!initialized) {
        return;
    }

    g_pipeline.reset();
    LOGI("GS resetado");
}

void shutdown() {
    if (!initialized) {
        return;
    }

    g_pipeline.shutdown();
    initialized = false;
    LOGI("GS finalizado");
}

void submitPacket(const std::vector<uint64_t>& packet) {
    gif::processPacket(packet);
}

const uint8_t* getFrameBuffer() {
    return frameBuffer;
}

} // namespace gs 