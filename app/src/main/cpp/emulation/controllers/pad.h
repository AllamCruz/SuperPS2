#ifndef SUPERPS2_PAD_H
#define SUPERPS2_PAD_H

#include <cstdint>

namespace pad {
    // Inicializa o sistema de pad
    bool init();

    // Atualiza estados internos a cada frame (chamar antes de memory::read)
    void update();

    // Define estados de botões/eixos (passado pelo InputManager)
    void setButtonState(int code, bool pressed);
    void setAxisValue(int axis, float value);

    // Leitura de 32‑bits do registrador de controle
    // port: 0 ou 1, regOffset: offset dentro do bloco
    uint32_t readRegister(int port, uint32_t regOffset);

    // Escrita (se necessário) — geralmente zeros
    void writeRegister(int port, uint32_t regOffset, uint32_t value);

    // Libera recursos
    void shutdown();
}

#endif // SUPERPS2_PAD_H 