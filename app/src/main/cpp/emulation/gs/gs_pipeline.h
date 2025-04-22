#ifndef SUPERPS2_GS_PIPELINE_H
#define SUPERPS2_GS_PIPELINE_H

#include <cstdint>
#include "renderer/vertex_buffer.h"

namespace gs {

    class GSPipeline {
    public:
        void reset();
        void writeRegister(uint8_t regIndex, uint64_t data);

    private:
        renderer::Vertex currentVertex{};
        bool hasRGBAQ = false;
        bool hasXYZ2 = false;
        uint8_t primitiveType = 0;
        renderer::VertexBuffer vertexBuffer;

        void submitVertexIfComplete();
    };

    extern GSPipeline g_pipeline;

}

#endif //SUPERPS2_GS_PIPELINE_H 