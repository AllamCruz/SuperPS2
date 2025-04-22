#include <jni.h>
#include <android/log.h>
#include "emulation/ps2core.h"

#define LOG_TAG "InputBridge"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace input {
    void onButtonEvent(int code, bool pressed) {
        if (!ps2::isInitialized()) {
            LOGE("PS2 core not initialized");
            return;
        }

        // Map button codes to PS2 controller state
        switch (code) {
            case 0: // UP
                ps2::setButtonState(ps2::BUTTON_UP, pressed);
                break;
            case 1: // DOWN
                ps2::setButtonState(ps2::BUTTON_DOWN, pressed);
                break;
            case 2: // LEFT
                ps2::setButtonState(ps2::BUTTON_LEFT, pressed);
                break;
            case 3: // RIGHT
                ps2::setButtonState(ps2::BUTTON_RIGHT, pressed);
                break;
            case 4: // X
                ps2::setButtonState(ps2::BUTTON_X, pressed);
                break;
            case 5: // CIRCLE
                ps2::setButtonState(ps2::BUTTON_CIRCLE, pressed);
                break;
            case 6: // TRIANGLE
                ps2::setButtonState(ps2::BUTTON_TRIANGLE, pressed);
                break;
            case 7: // SQUARE
                ps2::setButtonState(ps2::BUTTON_SQUARE, pressed);
                break;
            case 8: // START
                ps2::setButtonState(ps2::BUTTON_START, pressed);
                break;
            case 9: // SELECT
                ps2::setButtonState(ps2::BUTTON_SELECT, pressed);
                break;
            default:
                LOGE("Unknown button code: %d", code);
                break;
        }

        LOGI("Button event: code=%d, pressed=%d", code, pressed);
    }
}

extern "C" {

JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_sendButtonEvent(
        JNIEnv* env,
        jobject thiz,
        jint code,
        jboolean pressed) {
    input::onButtonEvent(code, pressed == JNI_TRUE);
}

} // extern "C" 