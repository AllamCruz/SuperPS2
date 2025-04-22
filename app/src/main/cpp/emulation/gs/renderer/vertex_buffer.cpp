#include "vertex_buffer.h"
#include <android/log.h>

#define TAG "VertexBuffer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

namespace renderer {

void VertexBuffer::beginPrimitive(uint8_t primType) {
    currentPrimitive = primType;
    vertices.clear();
    LOGI("Iniciando primitiva tipo %u", primType);
}

void VertexBuffer::addVertex(const Vertex& v) {
    vertices.push_back(v);
    LOGI("Vértice adicionado: X=%.2f, Y=%.2f, Z=%.2f RGBA=(%d,%d,%d,%d)",
        v.x, v.y, v.z, v.r, v.g, v.b, v.a);
}

void VertexBuffer::endPrimitive() {
    LOGI("Primitiva finalizada. Total de vértices: %zu", vertices.size());
    // Aqui futuramente enviaremos os vértices para a GPU
}

const std::vector<Vertex>& VertexBuffer::getVertices() const {
    return vertices;
}

void VertexBuffer::clear() {
    vertices.clear();
}

} 