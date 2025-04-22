#ifndef SUPERPS2_MEMORYCARD_H
#define SUPERPS2_MEMORYCARD_H

#include <string>
#include <vector>
#include <cstdint>

namespace memorycard {
    // Inicializa o sistema de memory cards (diretório emulado)
    bool init(const std::string& basePath);

    // Cria ou abre um memory card (arquivo .ps2mem)
    bool openCard(uint8_t slot);  // slot: 0 ou 1

    // Lista todos os saves no cartão aberto
    std::vector<std::string> listSaves();

    // Lê bloco de 128 bytes de um save pelo nome
    bool readSave(const std::string& saveName, uint8_t blockIndex, uint8_t* outData);

    // Grava bloco de 128 bytes de um save (cria se não existir)
    bool writeSave(const std::string& saveName, uint8_t blockIndex, const uint8_t* data);

    // Fecha o cartão e libera recursos
    void shutdown();
}

#endif // SUPERPS2_MEMORYCARD_H 