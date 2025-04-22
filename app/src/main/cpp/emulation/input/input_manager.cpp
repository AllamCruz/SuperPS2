#include "input_manager.h"
#include <array>
#include <atomic>
#include <android/log.h>

#define TAG "InputManager"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

namespace input {

    static std::array<std::atomic<bool>, MAX_BUTTONS> buttonStates;
    static std::array<std::atomic<float>, MAX_AXES>   axisValues;

    bool init() {
        for (auto &b : buttonStates) b.store(false);
        for (auto &a : axisValues)  a.store(0.0f);
        LOGI("InputManager initialized.");
        return true;
    }

    void setButtonState(int code, bool pressed) {
        if (code >= 0 && code < MAX_BUTTONS) {
            buttonStates[code].store(pressed);
        }
    }

    void setAxisValue(int code, float value) {
        if (code >= 0 && code < MAX_AXES) {
            axisValues[code].store(value);
        }
    }

    void update() {
        // In pure interpreter, possibly nothing to do here.
        // If you want debouncing or edge-detection, apply it here.
    }

    bool getButtonState(int code) {
        if (code >= 0 && code < MAX_BUTTONS) {
            return buttonStates[code].load();
        }
        return false;
    }

    float getAxisValue(int code) {
        if (code >= 0 && code < MAX_AXES) {
            return axisValues[code].load();
        }
        return 0.0f;
    }

    void shutdown() {
        LOGI("InputManager shutdown.");
    }

} // namespace input 