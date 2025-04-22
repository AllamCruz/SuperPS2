#include "renderer.h"
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#define LOG_TAG "Renderer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace renderer {
    static EGLDisplay display = EGL_NO_DISPLAY;
    static EGLSurface surface = EGL_NO_SURFACE;
    static EGLContext context = EGL_NO_CONTEXT;
    static ANativeWindow* window = nullptr;
    static bool initialized = false;

    bool init(ANativeWindow* win, int width, int height) {
        if (initialized) {
            LOGI("Renderer already initialized");
            return true;
        }

        window = win;
        if (!window) {
            LOGE("Invalid window");
            return false;
        }

        // Initialize EGL
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (display == EGL_NO_DISPLAY) {
            LOGE("Failed to get EGL display");
            return false;
        }

        if (!eglInitialize(display, nullptr, nullptr)) {
            LOGE("Failed to initialize EGL");
            return false;
        }

        // Configure EGL
        const EGLint attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
        };

        EGLConfig config;
        EGLint numConfigs;
        if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs)) {
            LOGE("Failed to choose EGL config");
            return false;
        }

        // Create surface
        surface = eglCreateWindowSurface(display, config, window, nullptr);
        if (surface == EGL_NO_SURFACE) {
            LOGE("Failed to create EGL surface");
            return false;
        }

        // Create context
        const EGLint contextAttribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
        };
        context = eglCreateContext(display, config, nullptr, contextAttribs);
        if (context == EGL_NO_CONTEXT) {
            LOGE("Failed to create EGL context");
            return false;
        }

        if (!eglMakeCurrent(display, surface, surface, context)) {
            LOGE("Failed to make EGL context current");
            return false;
        }

        // Set viewport
        glViewport(0, 0, width, height);

        initialized = true;
        LOGI("Renderer initialized successfully");
        return true;
    }

    void render(const uint8_t* framebuffer, int width, int height) {
        if (!initialized) {
            LOGE("Renderer not initialized");
            return;
        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO: Implement actual rendering of the framebuffer
        // This is a placeholder that just renders a solid color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers
        eglSwapBuffers(display, surface);
    }

    void shutdown() {
        if (!initialized) {
            return;
        }

        if (display != EGL_NO_DISPLAY) {
            eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            if (context != EGL_NO_CONTEXT) {
                eglDestroyContext(display, context);
            }
            if (surface != EGL_NO_SURFACE) {
                eglDestroySurface(display, surface);
            }
            eglTerminate(display);
        }

        display = EGL_NO_DISPLAY;
        surface = EGL_NO_SURFACE;
        context = EGL_NO_CONTEXT;
        window = nullptr;
        initialized = false;
        LOGI("Renderer shut down");
    }
} 