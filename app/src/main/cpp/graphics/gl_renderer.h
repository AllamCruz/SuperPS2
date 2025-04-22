#ifndef SUPERPS2_GL_RENDERER_H
#define SUPERPS2_GL_RENDERER_H

#include <GLES3/gl3.h>
#include "../emulation/gs/renderer/vertex_buffer.h"

namespace graphics {

class GLRenderer {
public:
    bool init();
    void render(const renderer::VertexBuffer& vertexBuffer);
    void shutdown();

private:
    GLuint programId = 0;
    GLuint vbo = 0;
    GLint attribPosition = -1;
    GLint attribColor = -1;

    GLuint compileShader(GLenum type, const char* source);
    bool createShaders();
};

extern GLRenderer glRenderer;

}

#endif //SUPERPS2_GL_RENDERER_H 