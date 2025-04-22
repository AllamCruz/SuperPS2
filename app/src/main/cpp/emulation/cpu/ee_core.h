#ifndef SUPERPS2_EE_CORE_H
#define SUPERPS2_EE_CORE_H

#include <cstdint>

namespace ee {

    // Inicializa registradores e estado da CPU
    void init();

    // Reinicia todos os registradores para o estado de power‑on
    void reset();

    // Executa um "frame" inteiro de ciclos de CPU
    void runFrame();

    // Para o loop de emulação e libera recursos
    void shutdown();

    // Imprime via logcat o estado completo da CPU (PC, HI, LO, GPRs)
    void printState();

}

#endif //SUPERPS2_EE_CORE_H