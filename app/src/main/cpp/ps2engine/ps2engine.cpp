#include "ps2engine.h"
#include "emulation/ps2core.h"
#include <android/log.h>
#include "gif/gif.h"
#include <fstream>

#define LOG_TAG "SuperPS2Engine"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace ps2engine {

    // Verifica se o arquivo existe
    bool fileExists(const std::string& path) {
        std::ifstream file(path);
        return file.good();
    }
    bool ps2core::init() {
        if (!ps2renderer::init()) return false;
        if (!gif::init()) { // Inicializa após o renderizador
            LOGE("Falha ao inicializar GIF.");
            return false;
        }
        return true;
    }
    
    void ps2core::shutdown() {
        gif::shutdown(); // Desliga antes do renderizador
        ps2renderer::shutdown();
    }
    // Inicia a engine de emulação (esqueleto)
    bool start(const std::string& isoPath) {
        LOGI("Inicializando motor PS2 com ISO: %s", isoPath.c_str());

        if (!fileExists(isoPath)) {
            LOGE("Arquivo ISO não encontrado: %s", isoPath.c_str());
            return false;
        }

        if (!ps2core::init(isoPath)) {
            LOGE("Erro ao inicializar núcleo da emulação.");
            return false;
        }

        ps2core::startLoop();
        // Em etapas futuras:
        // - Inicializar sistema (BIOS, memória, etc)
        // - Preparar plugins
        // - Carregar ISO
        // - Iniciar thread de execução

        LOGI("ISO validado com sucesso. Pronto para iniciar emulação.");
        return true;
    }
}
