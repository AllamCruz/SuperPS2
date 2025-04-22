#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>

namespace renderer {
    bool init(void* window, int width, int height);
    void render(const uint8_t* framebuffer, int width, int height);
    void shutdown();
}

#endif // RENDERER_H 