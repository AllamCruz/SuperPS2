#ifndef SUPERPS2_VU1_H
#define SUPERPS2_VU1_H

#include <cstdint>
#include <string>

namespace vu1 {
    // Inicializa VU1 (aloca registradores, micro‑RAM, scratchpad)
    bool init();

    // Carrega microcódigo (binário) para VU1
    bool loadMicroProgram(const std::string& path);

    // Executa um frame de micro‑instruções (stub)
    void runFrame();

    // Reseta estado interno
    void reset();

    // Libera recursos
    void shutdown();
}

#endif // SUPERPS2_VU1_H 