#ifndef SUPERPS2_SPU2_H
#define SUPERPS2_SPU2_H

#include <cstdint>
#include <string>

namespace spu2 {
    // Inicializa SPU2, aloca buffers
    bool init();

    // Carrega firmware (caso necessário)
    bool loadFirmware(const std::string& path);

    // Processa um "frame" de áudio (deve ser chamado a cada ciclo de EE)
    void runFrame();

    // Grava valor em um registrador da SPU2 (via MMIO)
    void writeRegister(uint32_t addr, uint16_t value);

    // Lê valor de um registrador da SPU2
    uint16_t readRegister(uint32_t addr);

    // Reset e liberação
    void reset();
    void shutdown();
}

#endif // SUPERPS2_SPU2_H 