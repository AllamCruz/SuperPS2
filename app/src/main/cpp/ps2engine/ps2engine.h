#ifndef SUPERPS2_PS2ENGINE_H
#define SUPERPS2_PS2ENGINE_H

#include <string>

namespace ps2engine {
    // Inicializa e tenta carregar o ISO
    bool start(const std::string& isoPath);
}

#endif //SUPERPS2_PS2ENGINE_H
