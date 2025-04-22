#include <jni.h>
#include "../emulation/state/state.h"
#include "../utils/log.h"
#include <string>

extern "C" {

JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_core_StateBridge_saveState(JNIEnv* env, jobject /* this */, jstring jpath) {
    const char* path = env->GetStringUTFChars(jpath, nullptr);
    if (!path) {
        LOGE("Falha ao obter string de caminho do Java");
        return JNI_FALSE;
    }

    bool result = state::saveState(path);
    env->ReleaseStringUTFChars(jpath, path);
    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_core_StateBridge_loadState(JNIEnv* env, jobject /* this */, jstring jpath) {
    const char* path = env->GetStringUTFChars(jpath, nullptr);
    if (!path) {
        LOGE("Falha ao obter string de caminho do Java");
        return JNI_FALSE;
    }

    bool result = state::loadState(path);
    env->ReleaseStringUTFChars(jpath, path);
    return result ? JNI_TRUE : JNI_FALSE;
}

} // extern "C" 