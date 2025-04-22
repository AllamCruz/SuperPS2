#include "gl_renderer.h"
#include <android/log.h>
#include <vector>

#define TAG "GLRenderer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

namespace graphics {

GLRenderer glRenderer;

const char* vertexShaderSrc = R"(#version 300 es
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
out vec4 vColor;
void main() {
    gl_Position = vec4(aPosition, 1.0);
    vColor = aColor;
})";

const char* fragmentShaderSrc = R"(#version 300 es
precision mediump float;
in vec4 vColor;
out vec4 fragColor;
void main() {
    fragColor = vColor;
})";

GLuint GLRenderer::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        LOGI("Shader compile error: %s", log);
        return 0;
    }
    return shader;
}

bool GLRenderer::createShaders() {
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    if (!vs || !fs) return false;

    programId = glCreateProgram();
    glAttachShader(programId, vs);
    glAttachShader(programId, fs);
    glLinkProgram(programId);

    GLint linked;
    glGetProgramiv(programId, GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[512];
        glGetProgramInfoLog(programId, sizeof(log), nullptr, log);
        LOGI("Shader link error: %s", log);
        return false;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    return true;
}

bool GLRenderer::init() {
    if (!createShaders()) return false;

    glGenBuffers(1, &vbo);
    return true;
}

void GLRenderer::render(const renderer::VertexBuffer& vertexBuffer) {
    glUseProgram(programId);

    const auto& vertices = vertexBuffer.getVertices();
    if (vertices.empty()) return;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(renderer::Vertex), vertices.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(renderer::Vertex), (void*)0);

    glEnableVertexAttribArray(1); // Color
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(renderer::Vertex), (void*)(3 * sizeof(float)));

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLRenderer::shutdown() {
    if (vbo) glDeleteBuffers(1, &vbo);
    if (programId) glDeleteProgram(programId);
}

} 