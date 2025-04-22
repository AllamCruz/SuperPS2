#ifndef SUPERPS2_CDVD_H
#define SUPERPS2_CDVD_H

#include <cstdint>
#include <vector>

namespace cdvd {
    // Status do drive
    enum class Status : uint8_t {
        NoDisc = 0,
        Idle,
        Seeking,
        Reading,
        Error
    };

    // Inicializa o subsistema CDVD (deve ser chamado após loadISO)
    bool init();

    // Seek para o setor LBA
    bool seek(uint32_t lba);

    // Ler setor LBA (2048 bytes) para o buffer fornecido
    bool readSector(uint32_t lba, uint8_t* outBuffer);

    // Retorna o status atual do drive
    Status getStatus();

    // Avança o estado interno (chamar cada frame antes de read)
    void update();

    // Libera recursos
    void shutdown();
}

#endif // SUPERPS2_CDVD_H 