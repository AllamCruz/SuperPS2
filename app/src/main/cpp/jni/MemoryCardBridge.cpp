#include <jni.h>
#include "../emulation/memorycard/memorycard.h"
#include "../utils/log.h"
#include <string>
#include <vector>

extern "C" {

JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_core_MemoryCardBridge_initCardSystem(JNIEnv* env, jobject /* this */, jstring basePath) {
    const char* path = env->GetStringUTFChars(basePath, nullptr);
    if (!path) {
        LOGE("Falha ao obter string de caminho do Java");
        return JNI_FALSE;
    }

    bool result = memorycard::init(path);
    env->ReleaseStringUTFChars(basePath, path);
    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_core_MemoryCardBridge_openCard(JNIEnv* /* env */, jobject /* this */, jint slot) {
    return memorycard::openCard(static_cast<uint8_t>(slot)) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobjectArray JNICALL
Java_com_superps2_emu_core_MemoryCardBridge_listSaves(JNIEnv* env, jobject /* this */) {
    std::vector<std::string> saves = memorycard::listSaves();
    jobjectArray result = env->NewObjectArray(saves.size(), env->FindClass("java/lang/String"), nullptr);
    
    for (size_t i = 0; i < saves.size(); ++i) {
        env->SetObjectArrayElement(result, i, env->NewStringUTF(saves[i].c_str()));
    }
    
    return result;
}

JNIEXPORT jbyteArray JNICALL
Java_com_superps2_emu_core_MemoryCardBridge_readSave(JNIEnv* env, jobject /* this */, jstring saveName, jint blockIndex) {
    const char* name = env->GetStringUTFChars(saveName, nullptr);
    if (!name) {
        LOGE("Falha ao obter nome do save do Java");
        return nullptr;
    }

    uint8_t data[128];
    bool success = memorycard::readSave(name, static_cast<uint8_t>(blockIndex), data);
    env->ReleaseStringUTFChars(saveName, name);

    if (!success) {
        return nullptr;
    }

    jbyteArray result = env->NewByteArray(128);
    env->SetByteArrayRegion(result, 0, 128, reinterpret_cast<jbyte*>(data));
    return result;
}

JNIEXPORT jboolean JNICALL
Java_com_superps2_emu_core_MemoryCardBridge_writeSave(JNIEnv* env, jobject /* this */, jstring saveName, jint blockIndex, jbyteArray data) {
    const char* name = env->GetStringUTFChars(saveName, nullptr);
    if (!name) {
        LOGE("Falha ao obter nome do save do Java");
        return JNI_FALSE;
    }

    jbyte* buffer = env->GetByteArrayElements(data, nullptr);
    bool success = memorycard::writeSave(name, static_cast<uint8_t>(blockIndex), reinterpret_cast<uint8_t*>(buffer));
    
    env->ReleaseByteArrayElements(data, buffer, 0);
    env->ReleaseStringUTFChars(saveName, name);
    
    return success ? JNI_TRUE : JNI_FALSE;
}

} // extern "C" 