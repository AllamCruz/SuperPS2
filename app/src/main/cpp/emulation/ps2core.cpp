#include "ps2core.h"
#include <android/log.h>
#include <thread>
#include <atomic>
#include <chrono>
#include "bios/bios.h"
#include "memory/memory.h"
#include "cpu/ee_core.h"

#define LOG_TAG "SuperPS2Core"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace ps2core {

    std::atomic<bool> running(false);
    std::string loadedISO;

    bool init(const std::string& isoPath) {
        LOGI("Inicializando PS2Core com ISO: %s", isoPath.c_str());
        loadedISO = isoPath;
    
        // Carrega a BIOS (caminho fixo por enquanto)
        std::string biosPath = "/sdcard/SuperPS2/bios/scph10000.bin";
        if (!bios::load(biosPath)) {
            LOGE("Falha ao carregar a BIOS. Abortando inicialização.");
            return false;
        if (!memory::init()) {
            LOGE("Falha ao inicializar memória principal.");
            return false;
        }
    
        // Em breve: inicializar outros subsistemas (memória, plugins, etc.)
        return true;
    }

    void emulationLoop() {
        LOGI("Iniciando loop principal de emulação...");

        const int frameTimeMs = 16; // ~60fps
        while (running.load()) {
            auto start = std::chrono::high_resolution_clock::now();

            LOGI("Executando ciclo de emulação...");

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start;

            if (elapsed.count() < frameTimeMs) {
                std::this_thread::sleep_for(std::chrono::milliseconds(frameTimeMs) - elapsed);
            }
        }

        LOGI("Loop principal finalizado.");
    }

    void init () {
        ee::init();
    }

    void startLoop() {
        running = true;
        std::thread loopThread(emulationLoop);
        loopThread.detach();
    }

    void stop() {
        ee::shutdown();
        running = false;
    }

    bool isRunning() {
        return running.load();
    }

}