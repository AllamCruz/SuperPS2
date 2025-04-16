#include "ps2renderer.h"
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <thread>
#include <chrono>

#define LOG_TAG "SuperPS2Renderer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace ps2renderer {

    static EGLDisplay display;
    static EGLSurface surface;
    static EGLContext context;
    static bool running = false;

    bool init(ANativeWindow* window) {
        EGLint configAttribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8,
            EGL_NONE
        };

        EGLint contextAttribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE
        };

        EGLConfig config;
        EGLint numConfigs;

        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        eglInitialize(display, nullptr, nullptr);
        eglChooseConfig(display, configAttribs, &config, 1, &numConfigs);
        surface = eglCreateWindowSurface(display, config, window, nullptr);
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
        eglMakeCurrent(display, surface, surface, context);

        running = true;
        LOGI("Renderizador OpenGL ES 3.0 iniciado com sucesso.");
        return true;
    }

    void renderLoop() {
        LOGI("Iniciando loop de renderização...");
        while (running) {
            glClearColor(0.0f, 0.1f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            eglSwapBuffers(display, surface);
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }

    void shutdown() {
        running = false;
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroyContext(display, context);
        eglDestroySurface(display, surface);
        eglTerminate(display);
        LOGI("Renderizador finalizado.");
    }

}