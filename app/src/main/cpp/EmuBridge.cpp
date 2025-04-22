#include <stdint.h>
#include <stddef.h>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cwchar>
#include <jni.h>
#include <string>
#include <android/log.h>
#include "ps2engine/ps2engine.h"
#include "osd/osd.h"
#include "emulation/options/engine_options.h"
#include "emulation/input/input_manager.h"
#include "audio/spu2.h"
#include "core/ps2_core.h"
#include "common.h"

#define LOG_TAG "SuperPS2-CPP"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Funções de controle do emulador
extern "C" JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_jni_PS2Bridge_startEmulation(JNIEnv *env, jobject, jstring isoPath) {
    const char *path = env->GetStringUTFChars(isoPath, 0);
    std::string isoPathStr(path);
    env->ReleaseStringUTFChars(isoPath, path);
    return static_cast<jboolean>(ps2engine::start(isoPathStr));
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_jni_PS2Bridge_stopEmulation(JNIEnv*, jobject) {
    return static_cast<jboolean>(ps2engine::stop());
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_jni_PS2Bridge_pauseEmulation(JNIEnv*, jobject) {
    return static_cast<jboolean>(ps2engine::pause());
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_jni_PS2Bridge_resumeEmulation(JNIEnv*, jobject) {
    return static_cast<jboolean>(ps2engine::resume());
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_jni_PS2Bridge_resetEmulation(JNIEnv*, jobject) {
    return static_cast<jboolean>(ps2engine::reset());
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_jni_PS2Bridge_isRunning(JNIEnv*, jobject) {
    return static_cast<jboolean>(ps2engine::isRunning());
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_jni_PS2Bridge_isPaused(JNIEnv*, jobject) {
    return static_cast<jboolean>(ps2engine::isPaused());
}

// Funções de estado
extern "C" JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_jni_PS2Bridge_saveState(JNIEnv* env, jobject, jstring savePath) {
    const char *path = env->GetStringUTFChars(savePath, 0);
    std::string pathStr(path);
    env->ReleaseStringUTFChars(savePath, path);
    return static_cast<jboolean>(ps2engine::saveState(pathStr));
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_jni_PS2Bridge_loadState(JNIEnv* env, jobject, jstring savePath) {
    const char *path = env->GetStringUTFChars(savePath, 0);
    std::string pathStr(path);
    env->ReleaseStringUTFChars(savePath, path);
    return static_cast<jboolean>(ps2engine::loadState(pathStr));
}

// Funções de áudio
extern "C" JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_jni_PS2Bridge_initAudio(JNIEnv*, jobject, jint sr, jint ch, jint bs) {
    return static_cast<jboolean>(spu2::init(sr, ch, bs));
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_writeAudio(JNIEnv* env, jobject, jshortArray data, jint num) {
    jshort* pcm = env->GetShortArrayElements(data, nullptr);
    spu2::writeSamples(reinterpret_cast<uint16_t*>(pcm), num);
    env->ReleaseShortArrayElements(data, pcm, 0);
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_shutdownAudio(JNIEnv*, jobject) {
    spu2::shutdown();
}

// Funções de OSD
extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_initOSD(JNIEnv*, jobject, jdouble targetFps) {
    osd::init(targetFps);
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_notifyFrame(JNIEnv*, jobject) {
    osd::notifyFrameRendered();
}

extern "C" JNIEXPORT jdouble JNICALL
Java_com_superps2_emu_jni_PS2Bridge_getFps(JNIEnv*, jobject) {
    return osd::getFps();
}

extern "C" JNIEXPORT jdouble JNICALL
Java_com_superps2_emu_jni_PS2Bridge_getSpeed(JNIEnv*, jobject) {
    return osd::getEmulationSpeed();
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_shutdownOSD(JNIEnv*, jobject) {
    osd::shutdown();
}

// Funções de configuração
extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_setResolutionScale(JNIEnv*, jobject, jfloat scale) {
    options::setResolutionScale(scale);
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_setFrameLimit(JNIEnv*, jobject, jint fps) {
    options::setFrameLimit(fps);
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_setVSyncEnabled(JNIEnv*, jobject, jboolean en) {
    options::setVSyncEnabled(en);
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_setMultiThreadEnabled(JNIEnv*, jobject, jboolean en) {
    options::setMultiThreadEnabled(en);
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_setNeonOptimizations(JNIEnv*, jobject, jboolean en) {
    options::setNeonOptimizations(en);
}

// Funções de entrada
extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_onAxisEvent(JNIEnv*, jobject, jint axis, jfloat value) {
    input::onAxisEvent(axis, value);
}

extern "C" JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_sendButtonEvent(JNIEnv*, jobject, jint code, jboolean pressed) {
    input::onButtonEvent(code, pressed);
} 