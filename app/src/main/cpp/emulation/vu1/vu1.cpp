#include "vu1.h"
#include "../memory/memory.h"
#include <android/log.h>
#include <vector>

#define TAG "SuperPS2_VU1"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

namespace vu1 {
    static std::vector<uint32_t> microRam; // 4 KB de micro‑RAM
    static bool initialized = false;

    bool init() {
        if (initialized) return true;
        microRam.assign(1024, 0);
        initialized = true;
        LOGI("VU1 initialized (4 KB micro‑RAM).");
        return true;
    }

    bool loadMicroProgram(const std::string& path) {
        // stub: ler binário em microRam
        LOGI("VU1 microprogram load: %s", path.c_str());
        return initialized;
    }

    void runFrame() {
        if (!initialized) return;
        // stub: processar instruções em microRam
        LOGI("VU1 runFrame() stub.");
    }

    void reset() {
        if (!initialized) return;
        std::fill(microRam.begin(), microRam.end(), 0);
        LOGI("VU1 reset.");
    }

    void shutdown() {
        if (!initialized) return;
        microRam.clear();
        initialized = false;
        LOGI("VU1 shutdown.");
    }
} 