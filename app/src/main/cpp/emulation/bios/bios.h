#ifndef SUPERPS2_BIOS_H
#define SUPERPS2_BIOS_H

#include <string>
#include <vector>

namespace bios {
    // Carrega o dump da BIOS a partir de um arquivo
    bool load(const std::string& biosPath);

    // Descarrega a BIOS da memória
    void unload();

    // Retorna os dados brutos da BIOS
    const std::vector<uint8_t>& getData();

    // Indica se a BIOS já foi carregada
    bool isLoaded();
}

#endif //SUPERPS2_BIOS_H
