#include <jni.h>
#include <android/log.h>
#include <string>
#include "emulation/ps2core.h"

#define LOG_TAG "EmulatorBridge"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace emulator {
    bool start(const std::string& isoPath) {
        if (ps2::isInitialized()) {
            LOGE("Emulator already running");
            return false;
        }

        LOGI("Starting emulator with ISO: %s", isoPath.c_str());
        if (!ps2::init()) {
            LOGE("Failed to initialize PS2 core");
            return false;
        }

        if (!ps2::loadISO(isoPath)) {
            LOGE("Failed to load ISO: %s", isoPath.c_str());
            ps2::shutdown();
            return false;
        }

        return true;
    }

    void pause() {
        if (!ps2::isInitialized()) {
            LOGE("Emulator not running");
            return;
        }

        LOGI("Pausing emulation");
        ps2::pause();
    }

    void resume() {
        if (!ps2::isInitialized()) {
            LOGE("Emulator not running");
            return;
        }

        LOGI("Resuming emulation");
        ps2::resume();
    }

    void reset() {
        if (!ps2::isInitialized()) {
            LOGE("Emulator not running");
            return;
        }

        LOGI("Resetting emulator");
        ps2::reset();
    }

    void stop() {
        if (!ps2::isInitialized()) {
            LOGE("Emulator not running");
            return;
        }

        LOGI("Stopping emulation");
        ps2::shutdown();
    }

    bool isRunning() {
        return ps2::isInitialized() && !ps2::isPaused();
    }

    bool isPaused() {
        return ps2::isInitialized() && ps2::isPaused();
    }
}

extern "C" {

JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_jni_PS2Bridge_startGame(
        JNIEnv* env,
        jobject thiz,
        jstring iso_path) {
    const char* nativePath = env->GetStringUTFChars(iso_path, nullptr);
    bool success = emulator::start(std::string(nativePath));
    env->ReleaseStringUTFChars(iso_path, nativePath);
    return success ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_pauseGame(
        JNIEnv* env,
        jobject thiz) {
    emulator::pause();
}

JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_resumeGame(
        JNIEnv* env,
        jobject thiz) {
    emulator::resume();
}

JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_resetGame(
        JNIEnv* env,
        jobject thiz) {
    emulator::reset();
}

JNIEXPORT void JNICALL
Java_com_superps2_emu_jni_PS2Bridge_stopGame(
        JNIEnv* env,
        jobject thiz) {
    emulator::stop();
}

JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_jni_PS2Bridge_isRunning(
        JNIEnv* env,
        jobject thiz) {
    return emulator::isRunning() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_jni_PS2Bridge_isPaused(
        JNIEnv* env,
        jobject thiz) {
    return emulator::isPaused() ? JNI_TRUE : JNI_FALSE;
}

} // extern "C" 