#include "state.h"
#include "../cpu/ee_core.h"
#include "../memory/memory.h"
#include "../dma/dma.h"
#include "../vif/vif.h"
#include "../gif/gif.h"
#include "../spu2/spu2.h"
#include "../../utils/log.h"
#include <zlib.h>

namespace state {

namespace {
    // Estrutura do cabeçalho do arquivo
    struct StateHeader {
        uint32_t magic;
        uint32_t version;
    };

    // Calcula CRC32 do conteúdo do arquivo
    uint32_t calculateCRC32(std::ifstream& file) {
        file.seekg(0);
        constexpr size_t BUFFER_SIZE = 8192;
        char buffer[BUFFER_SIZE];
        uint32_t crc = crc32(0L, Z_NULL, 0);
        
        while (file.good()) {
            file.read(buffer, BUFFER_SIZE);
            size_t bytesRead = file.gcount();
            if (bytesRead > 0) {
                crc = crc32(crc, reinterpret_cast<const Bytef*>(buffer), bytesRead);
            }
        }
        
        return crc;
    }
}

bool saveState(const std::string& filePath) {
    LOGI("Salvando estado da emulação em: %s", filePath.c_str());
    
    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        LOGE("Falha ao abrir arquivo de estado para escrita");
        return false;
    }

    try {
        // Escreve cabeçalho do arquivo
        StateHeader header{STATE_MAGIC, CURRENT_VERSION};
        file.write(reinterpret_cast<const char*>(&header), sizeof(header));

        // Salva estado da CPU
        LOGI("Salvando estado da CPU...");
        if (!ee::serializeState(file)) {
            throw std::runtime_error("Falha ao salvar estado da CPU");
        }

        // Salva estado do COP0
        LOGI("Salvando estado do COP0...");
        if (!cop0::serializeState(file)) {
            throw std::runtime_error("Falha ao salvar estado do COP0");
        }

        // Salva estado da Memória
        LOGI("Salvando estado da Memória...");
        if (!memory::serializeState(file)) {
            throw std::runtime_error("Falha ao salvar estado da Memória");
        }

        // Salva estado do DMA
        LOGI("Salvando estado do DMA...");
        if (!dma::serializeState(file)) {
            throw std::runtime_error("Falha ao salvar estado do DMA");
        }

        // Salva estado do VIF
        LOGI("Salvando estado do VIF...");
        if (!vif::serializeState(file)) {
            throw std::runtime_error("Falha ao salvar estado do VIF");
        }

        // Salva estado do GIF
        LOGI("Salvando estado do GIF...");
        if (!gif::serializeState(file)) {
            throw std::runtime_error("Falha ao salvar estado do GIF");
        }

        // Salva estado do SPU2
        LOGI("Salvando estado do SPU2...");
        if (!spu2::serializeState(file)) {
            throw std::runtime_error("Falha ao salvar estado do SPU2");
        }

        // Escreve marcador END
        SectionHeader endHeader{SectionID::END, 0};
        file.write(reinterpret_cast<const char*>(&endHeader), sizeof(endHeader));

        // Calcula e escreve CRC32
        uint32_t crc = calculateCRC32(file);
        file.seekp(0, std::ios::end);
        file.write(reinterpret_cast<const char*>(&crc), sizeof(crc));

        LOGI("Salvamento de estado concluído com sucesso");
        return true;

    } catch (const std::exception& e) {
        LOGE("Erro ao salvar estado: %s", e.what());
        return false;
    }
}

bool loadState(const std::string& filePath) {
    LOGI("Carregando estado da emulação de: %s", filePath.c_str());
    
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        LOGE("Falha ao abrir arquivo de estado para leitura");
        return false;
    }

    try {
        // Lê e verifica cabeçalho
        StateHeader header;
        file.read(reinterpret_cast<char*>(&header), sizeof(header));
        
        if (header.magic != STATE_MAGIC) {
            throw std::runtime_error("Formato de arquivo de estado inválido");
        }
        
        if (header.version > CURRENT_VERSION) {
            throw std::runtime_error("Versão do arquivo de estado não suportada");
        }

        // Carrega estado da CPU
        LOGI("Carregando estado da CPU...");
        if (!ee::deserializeState(file)) {
            throw std::runtime_error("Falha ao carregar estado da CPU");
        }

        // Carrega estado do COP0
        LOGI("Carregando estado do COP0...");
        if (!cop0::deserializeState(file)) {
            throw std::runtime_error("Falha ao carregar estado do COP0");
        }

        // Carrega estado da Memória
        LOGI("Carregando estado da Memória...");
        if (!memory::deserializeState(file)) {
            throw std::runtime_error("Falha ao carregar estado da Memória");
        }

        // Carrega estado do DMA
        LOGI("Carregando estado do DMA...");
        if (!dma::deserializeState(file)) {
            throw std::runtime_error("Falha ao carregar estado do DMA");
        }

        // Carrega estado do VIF
        LOGI("Carregando estado do VIF...");
        if (!vif::deserializeState(file)) {
            throw std::runtime_error("Falha ao carregar estado do VIF");
        }

        // Carrega estado do GIF
        LOGI("Carregando estado do GIF...");
        if (!gif::deserializeState(file)) {
            throw std::runtime_error("Falha ao carregar estado do GIF");
        }

        // Carrega estado do SPU2
        LOGI("Carregando estado do SPU2...");
        if (!spu2::deserializeState(file)) {
            throw std::runtime_error("Falha ao carregar estado do SPU2");
        }

        // Verifica marcador END
        SectionHeader endHeader;
        file.read(reinterpret_cast<char*>(&endHeader), sizeof(endHeader));
        if (endHeader.id != SectionID::END) {
            throw std::runtime_error("Arquivo de estado corrompido: marcador END ausente");
        }

        // Verifica CRC32
        uint32_t storedCRC;
        file.seekg(-static_cast<int>(sizeof(uint32_t)), std::ios::end);
        file.read(reinterpret_cast<char*>(&storedCRC), sizeof(storedCRC));
        
        uint32_t calculatedCRC = calculateCRC32(file);
        if (calculatedCRC != storedCRC) {
            throw std::runtime_error("Arquivo de estado corrompido: CRC não corresponde");
        }

        LOGI("Carregamento de estado concluído com sucesso");
        return true;

    } catch (const std::exception& e) {
        LOGE("Erro ao carregar estado: %s", e.what());
        return false;
    }
}

} // namespace state 