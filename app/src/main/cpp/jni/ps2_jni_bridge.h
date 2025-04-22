#pragma once

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

// Chamada principal do emulador (loop)
JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_core_PS2Bridge_loadISO(JNIEnv* env, jobject thiz, jstring path);

JNIEXPORT void JNICALL
Java_com_superps2_emu_core_PS2Bridge_runEmulator(JNIEnv* env, jobject thiz);

JNIEXPORT void JNICALL
Java_com_superps2_emu_core_PS2Bridge_pauseEmulator(JNIEnv* env, jobject thiz);

JNIEXPORT void JNICALL
Java_com_superps2_emu_core_PS2Bridge_resumeEmulator(JNIEnv* env, jobject thiz);

JNIEXPORT void JNICALL
Java_com_superps2_emu_core_PS2Bridge_stopEmulator(JNIEnv* env, jobject thiz);

#ifdef __cplusplus
}
#endif 