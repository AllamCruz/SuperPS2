#include "gs_pipeline.h"
#include "gs_packet_decoder.h"
#include <android/log.h>

#define TAG "GSPipeline"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

namespace gs {

GSPipeline g_pipeline;

void GSPipeline::reset() {
    vertexBuffer.clear();
    hasRGBAQ = false;
    hasXYZ2 = false;
}

void GSPipeline::writeRegister(uint8_t regIndex, uint64_t data) {
    switch (regIndex) {
        case PRIM:
            primitiveType = data & 0x07; // tipo da primitiva (triângulo, linha etc.)
            vertexBuffer.beginPrimitive(primitiveType);
            break;

        case RGBAQ:
            currentVertex.r = (data >> 0) & 0xFF;
            currentVertex.g = (data >> 8) & 0xFF;
            currentVertex.b = (data >> 16) & 0xFF;
            currentVertex.a = (data >> 24) & 0xFF;
            hasRGBAQ = true;
            break;

        case XYZ2:
            currentVertex.x = static_cast<float>((data >>  0) & 0xFFFF) / 16.0f;
            currentVertex.y = static_cast<float>((data >> 16) & 0xFFFF) / 16.0f;
            currentVertex.z = static_cast<float>((data >> 32) & 0xFFFFFF);
            hasXYZ2 = true;
            break;

        case A_D:
            {
                uint8_t targetReg = (data >> 0) & 0xFF;
                uint64_t value = (data >> 32);
                writeRegister(targetReg, value);
            }
            break;
    }

    submitVertexIfComplete();
}

void GSPipeline::submitVertexIfComplete() {
    if (hasRGBAQ && hasXYZ2) {
        vertexBuffer.addVertex(currentVertex);
        hasRGBAQ = false;
        hasXYZ2 = false;
    }
}

} 