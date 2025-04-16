#ifndef SUPERPS2_PS2RENDERER_H
#define SUPERPS2_PS2RENDERER_H

#include <android/native_window.h>

namespace ps2renderer {
    bool init(ANativeWindow* window);
    void renderLoop();
    void shutdown();
}

#endif //SUPERPS2_PS2RENDERER_H