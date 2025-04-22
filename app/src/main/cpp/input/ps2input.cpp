#include "ps2input.h"
#include <android/log.h>

#define LOG_TAG "PS2INPUT"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

ControllerState gControllerState = {};

extern "C" {

JNIEXPORT void JNICALL Java_com_superps2_emu_core_NativeInput_onButtonEvent(JNIEnv*, jobject, jint buttonCode, jboolean pressed) {
    if (buttonCode >= 0 && buttonCode < (int)gControllerState.buttons.size()) {
        gControllerState.buttons[buttonCode] = pressed;
        LOGI("Button %d %s", buttonCode, pressed ? "pressed" : "released");
    }
}

JNIEXPORT void JNICALL Java_com_superps2_emu_core_NativeInput_onAxisEvent(JNIEnv*, jobject, jint axisCode, jfloat value) {
    if (axisCode >= 0 && axisCode < (int)gControllerState.axes.size()) {
        gControllerState.axes[axisCode] = value;
        LOGI("Axis %d value=%.2f", axisCode, value);
    }
}

JNIEXPORT jlong JNICALL Java_com_superps2_emu_core_NativeInput_getControllerState(JNIEnv*, jobject) {
    return reinterpret_cast<jlong>(&gControllerState);
}

} // extern "C"
