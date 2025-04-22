#ifndef SUPERPS2_GS_H
#define SUPERPS2_GS_H

#include <cstdint>
#include <vector>

namespace gs {
    // Inicializa o GS core (chamar em ps2core::init)
    bool init(int screenWidth, int screenHeight);

    // Reseta estado interno
    void reset();

    // Escreve 32‑bits em um registrador GS (MMIO)
    void writeRegister(uint32_t addr, uint32_t value);

    // Lê 32‑bits de um registrador GS (se aplicável)
    uint32_t readRegister(uint32_t addr);

    // Submete um bloco de comandos (FIFO de GIF)
    void submitPacket(const std::vector<uint64_t>& packet);

    // Renderiza o quadro atual (chamar antes de eglSwapBuffers)
    // Fornece framebuffer raw RGBA para o renderer
    const uint8_t* getFrameBuffer();

    // Encerra o GS core (chamar em ps2core::shutdown)
    void shutdown();
}

#endif // SUPERPS2_GS_H 