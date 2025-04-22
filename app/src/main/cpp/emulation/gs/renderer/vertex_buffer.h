#ifndef SUPERPS2_VERTEX_BUFFER_H
#define SUPERPS2_VERTEX_BUFFER_H

#include <vector>
#include <cstdint>

namespace renderer {

    struct Vertex {
        float x, y, z;
        uint8_t r, g, b, a;
    };

    class VertexBuffer {
    public:
        void beginPrimitive(uint8_t primType);
        void addVertex(const Vertex& v);
        void endPrimitive();

        const std::vector<Vertex>& getVertices() const;
        void clear();

    private:
        std::vector<Vertex> vertices;
        uint8_t currentPrimitive = 0;
    };

}

#endif // SUPERPS2_VERTEX_BUFFER_H 