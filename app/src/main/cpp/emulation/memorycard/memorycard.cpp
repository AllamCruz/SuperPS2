#include "memorycard.h"
#include <fstream>
#include <filesystem>
#include <android/log.h>

#define TAG "SuperPS2_MemoryCard"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

namespace memorycard {
    static std::string g_basePath;
    static std::string g_cardPath;
    static const size_t BLOCK_SIZE = 128;

    bool init(const std::string& basePath) {
        g_basePath = basePath;
        std::filesystem::create_directories(g_basePath);
        LOGI("MemoryCard base path: %s", g_basePath.c_str());
        return true;
    }

    bool openCard(uint8_t slot) {
        g_cardPath = g_basePath + "/card" + std::to_string(slot) + ".ps2mem";
        if (!std::filesystem::exists(g_cardPath)) {
            // cria arquivo de cartão vazio (128KB)
            std::ofstream ofs(g_cardPath, std::ios::binary);
            std::vector<uint8_t> empty(128 * 1024, 0xFF);
            ofs.write(reinterpret_cast<char*>(empty.data()), empty.size());
            ofs.close();
            LOGI("Created new memory card: %s", g_cardPath.c_str());
        } else {
            LOGI("Opened existing memory card: %s", g_cardPath.c_str());
        }
        return true;
    }

    std::vector<std::string> listSaves() {
        // stub: retorno de saves existentes (poderia ler diretório interno)
        // Aqui, buscamos nomes de diretórios ou índices salvos
        return { "Save01", "Save02" };
    }

    bool readSave(const std::string& saveName, uint8_t blockIndex, uint8_t* outData) {
        std::ifstream ifs(g_cardPath, std::ios::binary);
        if (!ifs.is_open()) { LOGE("Failed to open card for read"); return false; }
        size_t offset = /* calcular a partir do saveName e blockIndex */ blockIndex * BLOCK_SIZE;
        ifs.seekg(offset);
        ifs.read(reinterpret_cast<char*>(outData), BLOCK_SIZE);
        ifs.close();
        return true;
    }

    bool writeSave(const std::string& saveName, uint8_t blockIndex, const uint8_t* data) {
        std::fstream fs(g_cardPath, std::ios::in | std::ios::out | std::ios::binary);
        if (!fs.is_open()) { LOGE("Failed to open card for write"); return false; }
        size_t offset = /* calcular a partir do saveName e blockIndex */ blockIndex * BLOCK_SIZE;
        fs.seekp(offset);
        fs.write(reinterpret_cast<const char*>(data), BLOCK_SIZE);
        fs.close();
        return true;
    }

    void shutdown() {
        LOGI("MemoryCard shutdown");
    }
} 