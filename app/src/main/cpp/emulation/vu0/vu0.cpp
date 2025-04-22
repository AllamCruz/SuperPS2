 #include "vu0.h"
#include "../memory/memory.h"
#include <android/log.h>
#include <vector>

#define TAG "SuperPS2_VU0"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

namespace vu0 {
    static std::vector<uint32_t> microRam; // 4 KB de micro‑RAM
    static bool initialized = false;

    bool init() {
        if (initialized) return true;
        microRam.assign(1024, 0); // 4 bytes × 1024 = 4 KB
        initialized = true;
        LOGI("VU0 initialized (4 KB micro‑RAM).");
        return true;
    }

    bool loadMicroProgram(const std::string& path) {
        // stub: futura leitura de arquivo binário para microRam
        LOGI("VU0 micro program load requested: %s", path.c_str());
        return initialized;
    }

    void runFrame() {
        if (!initialized) return;
        // stub: aqui você percorreria microRam executando instruções
        LOGI("VU0 runFrame() stub executed.");
    }

    void reset() {
        if (!initialized) return;
        std::fill(microRam.begin(), microRam.end(), 0);
        LOGI("VU0 reset.");
    }

    void shutdown() {
        if (!initialized) return;
        microRam.clear();
        initialized = false;
        LOGI("VU0 shutdown.");
    }
}