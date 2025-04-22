#ifndef SUPERPS2_VU0_H
#define SUPERPS2_VU0_H

#include <cstdint>
#include <string>

namespace vu0 {
    // Inicializa o VU0 (aloca registradores, limpa micro‑RAM)
    bool init();

    // Carrega microcódigo (binário) para VU0
    bool loadMicroProgram(const std::string& path);

    // Executa um frame de micro‑instruções (stub: loop vazio)
    void runFrame();

    // Reseta estado interno
    void reset();

    // Libera recursos
    void shutdown();
}

#endif // SUPERPS2_VU0_H 