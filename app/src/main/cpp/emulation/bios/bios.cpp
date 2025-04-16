#include "bios.h"
#include <fstream>
#include <android/log.h>

#define LOG_TAG "SuperPS2BIOS"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace bios {

    static std::vector<uint8_t> biosData;
    static bool loaded = false;

    bool load(const std::string& biosPath) {
        std::ifstream file(biosPath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            LOGE("Não foi possível abrir a BIOS em: %s", biosPath.c_str());
            return false;
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        biosData.resize(static_cast<size_t>(size));
        if (!file.read(reinterpret_cast<char*>(biosData.data()), size)) {
            LOGE("Erro ao ler dados da BIOS.");
            return false;
        }

        loaded = true;
        LOGI("BIOS carregada com sucesso. Tamanho: %ld bytes", size);
        return true;
    }

    void unload() {
        biosData.clear();
        loaded = false;
        LOGI("BIOS descarregada.");
    }

    const std::vector<uint8_t>& getData() {
        return biosData;
    }

    bool isLoaded() {
        return loaded;
    }

}
