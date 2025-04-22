#ifndef SUPERPS2_GIF_H
#define SUPERPS2_GIF_H

#include <cstdint>
#include <vector>

namespace gif {
    // Inicializa/reset GIF state
    void reset();

    // Processa uma lista de QWORDS (GIF packets)
    void processPacket(const std::vector<uint64_t>& packet);
}

#endif // SUPERPS2_GIF_H 