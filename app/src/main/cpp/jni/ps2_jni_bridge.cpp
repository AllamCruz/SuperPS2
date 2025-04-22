#include "ps2_jni_bridge.h"
#include "../emulation/ps2core.h"
#include <android/log.h>

#define LOG_TAG "PS2JNI"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_core_PS2Bridge_loadISO(JNIEnv* env, jobject thiz, jstring jpath) {
    const char* utf = env->GetStringUTFChars(jpath, nullptr);
    bool ok = ps2core::loadISO(std::string(utf));
    env->ReleaseStringUTFChars(jpath, utf);
    return ok ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_com_superps2_emu_core_PS2Bridge_runEmulator(JNIEnv* env, jobject thiz) {
    ps2core::run();
}

JNIEXPORT void JNICALL
Java_com_superps2_emu_core_PS2Bridge_pauseEmulator(JNIEnv* env, jobject thiz) {
    ps2core::pauseEmulation();
}

JNIEXPORT void JNICALL
Java_com_superps2_emu_core_PS2Bridge_resumeEmulator(JNIEnv* env, jobject thiz) {
    ps2core::resumeEmulation();
}

JNIEXPORT void JNICALL
Java_com_superps2_emu_core_PS2Bridge_stopEmulator(JNIEnv* env, jobject thiz) {
    ps2core::stopEmulation();
} 