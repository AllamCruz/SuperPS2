#include <jni.h>
#include "renderer.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>

extern "C" {

JNIEXPORT jboolean JNICALL
Java_com_example_superps2_RendererBridge_initRenderer(
    JNIEnv* env, jclass clazz, jobject surface, jint width, jint height) {
    ANativeWindow* window = ANativeWindow_fromSurface(env, surface);
    if (!window) return JNI_FALSE;
    bool success = renderer::init(window, width, height);
    ANativeWindow_release(window);
    return success ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_com_example_superps2_RendererBridge_renderFrame(
    JNIEnv* env, jclass clazz, jbyteArray framebuffer, jint width, jint height) {
    jbyte* fbData = env->GetByteArrayElements(framebuffer, nullptr);
    renderer::renderFrame(reinterpret_cast<uint8_t*>(fbData), width, height);
    env->ReleaseByteArrayElements(framebuffer, fbData, 0);
}

JNIEXPORT void JNICALL
Java_com_example_superps2_RendererBridge_shutdownRenderer(
    JNIEnv* env, jclass clazz) {
    renderer::shutdown();
}

} 