#ifndef SUPERPS2_PS2INPUT_H
#define SUPERPS2_PS2INPUT_H

#include <jni.h>
#include <array>

struct ControllerState {
    std::array<bool, 8> buttons;   // PS2 buttons
    std::array<float, 4> axes;     // LX, LY, RX, RY
};

extern ControllerState gControllerState;

extern "C" {
JNIEXPORT void JNICALL Java_com_superps2_emu_core_NativeInput_onButtonEvent(JNIEnv*, jobject, jint buttonCode, jboolean pressed);
JNIEXPORT void JNICALL Java_com_superps2_emu_core_NativeInput_onAxisEvent(JNIEnv*, jobject, jint axisCode, jfloat value);
JNIEXPORT jlong JNICALL Java_com_superps2_emu_core_NativeInput_getControllerState(JNIEnv*, jobject);
}

#endif //SUPERPS2_PS2INPUT_H
