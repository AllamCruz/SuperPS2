#ifndef SUPERPS2_OSD_H
#define SUPERPS2_OSD_H

#include <cstdint>

namespace osd {
    // Deve ser chamada a cada frame renderizado
    void notifyFrameRendered();

    // Inicializa o sistema de medição (chamar em init)
    void init(double targetFps);

    // Retorna o FPS calculado no último segundo
    double getFps();

    // Retorna a velocidade da emulação: (frames_emulados / frames_reais) * 100%
    double getEmulationSpeed();

    // Limpa recursos (chamar em shutdown)
    void shutdown();
}

#endif // SUPERPS2_OSD_H 