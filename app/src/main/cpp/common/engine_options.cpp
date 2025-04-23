#include "engine_options.h"
#include <android/log.h>

#define TAG "EngineOptions"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

namespace options {
    static float resolutionScale = 1.0f;
    static int frameLimit = 60;
    static bool vsync = true;
    static bool multiThread = true;
    static bool neon = true;

    void init() {
        LOGI("EngineOptions initialized.");
    }

    void setResolutionScale(float scale) {
        resolutionScale = scale;
        LOGI("Resolution scale set to %.2f", resolutionScale);
        // Aqui deve repassar para o renderer redimensionar framebuffer
    }

    void setFrameLimit(int fps) {
        frameLimit = fps;
        LOGI("Frame limit set to %d FPS", frameLimit);
    }

    void setVSyncEnabled(bool enabled) {
        vsync = enabled;
        LOGI("VSync %s", vsync ? "enabled" : "disabled");
    }

    void setMultiThreadEnabled(bool enabled) {
        multiThread = enabled;
        LOGI("Multi-thread %s", multiThread ? "enabled" : "disabled");
        // Ajustar threads do ps2core/render se necessário
    }

    void setNeonOptimizations(bool enabled) {
        neon = enabled;
        LOGI("NEON optimizations %s", neon ? "enabled" : "disabled");
        // Controlar paths de código NEON vs. genérico
    }

    void shutdown() {
        LOGI("EngineOptions shutdown.");
    }
} 