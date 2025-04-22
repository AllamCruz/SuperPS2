#include "osd.h"
#include <chrono>
#include <atomic>

namespace osd {
    static double targetFps = 60.0;
    static std::atomic<int> frameCount{0};
    static std::atomic<int> emuFrameCount{0};
    static double lastFps = 0.0;
    static std::chrono::steady_clock::time_point lastTime;

    void init(double tf) {
        targetFps = tf;
        frameCount = emuFrameCount = 0;
        lastFps = 0.0;
        lastTime = std::chrono::steady_clock::now();
    }

    void notifyFrameRendered() {
        emuFrameCount++;
        frameCount++;
        auto now = std::chrono::steady_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count();
        if (diff >= 1000) {
            lastFps = (frameCount * 1000.0) / diff;
            frameCount = 0;
            lastTime = now;
        }
    }

    double getFps() {
        return lastFps;
    }

    double getEmulationSpeed() {
        // se targetFps é a taxa ideal, relação de frames processados
        return (lastFps / targetFps) * 100.0;
    }

    void shutdown() {
        // nada para liberar
    }
} 