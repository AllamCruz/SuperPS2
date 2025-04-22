#ifndef SUPERPS2_IOP_H
#define SUPERPS2_IOP_H

#include <string>
#include <cstdint>

namespace iop {
    // Inicializa o IOP (registradores, RAM de 2 MB, periféricos)
    bool init();

    // Carrega microcódigo / BIOS do IOP (iop_rom.bin)
    bool loadBios(const std::string& path);

    // Executa um "frame" de IOP — processo de instruções MIPS R3000
    void runFrame();

    // Encaminha eventuais interrupções ou comandos do DMA
    void handleDmaInterrupt(int channel);

    // Reseta estado
    void reset();

    // Libera recursos
    void shutdown();
}

#endif // SUPERPS2_IOP_H 