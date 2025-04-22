#ifndef SUPERPS2_STATE_H
#define SUPERPS2_STATE_H

#include <string>
#include <cstdint>
#include <fstream>

namespace state {
    // Versão atual do formato de arquivo de estado
    constexpr uint32_t CURRENT_VERSION = 1;
    
    // Cabeçalho mágico para arquivos de estado ("SPS2")
    constexpr uint32_t STATE_MAGIC = 0x53505332;

    // IDs de seção para diferentes componentes
    enum class SectionID : uint32_t {
        CPU     = 0x00435055, // "CPU\0"
        COP0    = 0x434F5030, // "COP0"
        MEMORY  = 0x4D454D52, // "MEMR"
        DMA     = 0x444D4120, // "DMA "
        VIF     = 0x56494620, // "VIF "
        GIF     = 0x47494620, // "GIF "
        SPU2    = 0x53505532, // "SPU2"
        END     = 0x454E4420  // "END "
    };

    // Estrutura do cabeçalho de seção
    struct SectionHeader {
        SectionID id;
        uint32_t size;
    };

    /**
     * @brief Captura o estado atual da emulação em um arquivo
     * @param filePath Caminho para salvar o arquivo de estado
     * @return true se bem sucedido, false caso contrário
     */
    bool saveState(const std::string& filePath);

    /**
     * @brief Carrega e restaura o estado da emulação de um arquivo
     * @param filePath Caminho para o arquivo de estado a ser carregado
     * @return true se bem sucedido, false caso contrário
     */
    bool loadState(const std::string& filePath);

    // Funções auxiliares para serialização
    namespace detail {
        template<typename T>
        bool writeSection(std::ofstream& file, SectionID id, const T* data, size_t size) {
            SectionHeader header{id, static_cast<uint32_t>(size)};
            file.write(reinterpret_cast<const char*>(&header), sizeof(header));
            file.write(reinterpret_cast<const char*>(data), size);
            return file.good();
        }

        template<typename T>
        bool readSection(std::ifstream& file, SectionID expectedId, T* data, size_t expectedSize) {
            SectionHeader header;
            file.read(reinterpret_cast<char*>(&header), sizeof(header));
            
            if (!file.good() || header.id != expectedId || header.size != expectedSize) {
                return false;
            }
            
            file.read(reinterpret_cast<char*>(data), expectedSize);
            return file.good();
        }
    }
}

#endif // SUPERPS2_STATE_H 