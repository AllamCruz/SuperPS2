#ifndef SUPERPS2_CHEAT_H
#define SUPERPS2_CHEAT_H

#include <string>
#include <vector>
#include <cstdint>

namespace cheat {
    struct Code {
        uint32_t address;
        uint32_t value;
        bool enabled;
    };

    // Inicializa o sistema de cheats
    bool init();

    // Carrega um arquivo de códigos (.cht) no formato GameShark/CodeBreaker
    bool loadCheatFile(const std::string& filePath);

    // Retorna a lista de códigos carregados
    const std::vector<Code>& getCodes();

    // Ativa/desativa um código
    void setEnabled(size_t index, bool enabled);

    // Aplica todos os cheats ativos na RAM (chamar a cada frame)
    void applyAll();

    // Libera recursos
    void shutdown();
}

#endif // SUPERPS2_CHEAT_H 