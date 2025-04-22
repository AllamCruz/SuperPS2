#include "dma/dma.h"
#include "ps2core.h"
#include <android/log.h>
#include <thread>
#include <atomic>
#include <chrono>
#include "bios/bios.h"
#include "memory/memory.h"
#include "cpu/ee_core.cpp"
#include "vif/vif.h"
#include "gif/gif.h"
#include "state/state.h"
#include "memorycard/memorycard.h"
#include "cheats/cheat.h"
#include "vu0/vu0.h"
#include "vu1/vu1.h"
#include "iop/iop.h"
#include "ee/interrupt.h"
#include "spu2/spu2.h"
#include "ee/memory.h"
#include "ee/cop0/cop0.h"
#include "ee/cpu/ee_core.h"
#include "ee/vu/vu0.h"
#include "ee/vu/vu1.h"
#include "ee/dma/dmac.h"
#include "ee/memory/ram.h"
#include "ee/cpu/gpr.h"
#include "ee/cpu/pc.h"
#include "ee/memory/mem_mainram.h"
#include "ee/cpu/decoder.h"
#include "ee/cpu/interpreter.h"
#include <fstream>
#include <vector>
#include <cstring>
#include "input/input_manager.h"
#include "cdvd/cdvd.h"
#include "gs/gs.h"

#define TAG "SuperPS2_Core"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

namespace ps2core {

    static std::atomic<bool> isPaused{false};
    static std::atomic<bool> isRunning{false};
    static constexpr int TARGET_FPS = 60;
    static constexpr std::chrono::milliseconds FRAME_TIME{1000 / TARGET_FPS};
    std::string loadedISO;
    static bool initialized = false;

    bool init(const std::string& isoPath) {
        if (initialized) return true;

        if (!memory::init()) {
            LOGE("Failed to initialize memory subsystem");
            return false;
        }

        if (!ee::memory::init()) {
            LOGE("Failed to initialize EE memory");
            return false;
        }

        if (!ee::cop0::init()) {
            LOGE("Failed to initialize COP0");
            return false;
        }

        if (!ee::cpu::init()) {
            LOGE("Failed to initialize EE CPU");
            return false;
        }

        if (!ee::vu0::init()) {
            LOGE("Failed to initialize VU0");
            return false;
        }

        if (!ee::vu1::init()) {
            LOGE("Failed to initialize VU1");
            return false;
        }

        if (!bios::load()) {
            LOGE("Failed to load BIOS");
            return false;
        }

        if (!iop::init()) {
            LOGE("Failed to init IOP");
            return false;
        }

        if (!iop::loadBios("/sdcard/SuperPS2/iop_rom.bin")) {
            LOGE("Failed to load IOP BIOS");
            return false;
        }

        if (!ee::interrupt::init()) {
            LOGE("Failed to initialize interrupt controller");
            return false;
        }

        if (!spu2::init()) {
            LOGE("Failed to initialize SPU2");
            return false;
        }

        if (!ee::dma::init()) {
            LOGE("Failed to initialize DMA Controller");
            return false;
        }

        if (!ee::cpu::initGPR()) {
            LOGE("Erro ao iniciar registradores GPR.");
            return false;
        }

        if (!ee::cpu::initPC()) {
            LOGE("Erro ao iniciar o Program Counter");
            return false;
        }

        if (!ee::memory::initMainRAM()) {
            LOGE("Erro ao iniciar memória principal.");
            return false;
        }

        if (!input::init()) {
            LOGI("Failed to initialize input manager");
            return false;
        }

        if (!cdvd::init()) {
            LOGE("Failed to initialize CDVD subsystem");
            return false;
        }

        if (!gs::init(FB_WIDTH, FB_HEIGHT)) {
            LOGE("Failed to initialize GS core");
            return false;
        }

        LOGI("Inicializando PS2Core com ISO: %s", isoPath.c_str());
        loadedISO = isoPath;
    
        initialized = true;
        LOGI("PS2 core initialized successfully");
        return true;
    }

    bool loadISO(const std::string& isoPath) {
        using namespace ee::memory;
        std::ifstream iso(isoPath, std::ios::binary | std::ios::ate);
        if (!iso.is_open()) {
            LOGE("Falha ao abrir ISO: %s", isoPath.c_str());
            return false;
        }

        auto size = iso.tellg();
        if (size > MAIN_RAM_SIZE) {
            LOGE("ISO muito grande (%lld bytes)", size);
            return false;
        }

        iso.seekg(0, std::ios::beg);
        std::vector<uint8_t> buffer((size_t)size);
        iso.read(reinterpret_cast<char*>(buffer.data()), size);
        iso.close();

        uint8_t* ramPtr = getRawPointer(0);
        std::memcpy(ramPtr, buffer.data(), size);
        LOGI("ISO carregada (%lld bytes) em 0x00000000", size);
        return true;
    }

    void pauseEmulation() {
        isPaused.store(true, std::memory_order_release);
        LOGI("Emulação pausada");
    }

    void resumeEmulation() {
        isPaused.store(false, std::memory_order_release);
        LOGI("Emulação retomada");
    }

    void stopEmulation() {
        isRunning.store(false, std::memory_order_release);
        LOGI("Emulação encerrada");
    }

    void run() {
        isRunning.store(true, std::memory_order_release);
        LOGI("Iniciando loop de emulação");

        auto lastFrameTime = std::chrono::high_resolution_clock::now();
        
        while (isRunning.load(std::memory_order_acquire)) {
            if (isPaused.load(std::memory_order_acquire)) {
                std::this_thread::sleep_for(FRAME_TIME);
                continue;
            }

            auto currentTime = std::chrono::high_resolution_clock::now();
            auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFrameTime);

            if (frameTime < FRAME_TIME) {
                std::this_thread::sleep_for(FRAME_TIME - frameTime);
                continue;
            }

            // TODO: Implementar ciclo de emulação
            // fetch, decode, execute, etc.

            lastFrameTime = currentTime;
        }

        LOGI("Loop de emulação finalizado");
    }

    void reset() {
        if (!initialized) return;

        vu0::reset();
        vu1::reset();
        iop::reset();
        ee::interrupt::reset();
        spu2::reset();
        memory::reset();
        ee::memory::reset();
        ee::cop0::reset();
        ee::cpu::reset();
        ee::vu0::reset();
        ee::vu1::reset();
        bios::reset();
        dma::reset();
        vif::reset();
        gif::reset();
        state::reset();
        memorycard::reset();
        cheat::reset();
        ee::dma::reset();
        ee::cpu::resetGPR();
        ee::cpu::resetPC();
        ee::memory::resetMainRAM();
        input::reset();
    }

    void shutdown() {
        if (!initialized) return;

        vu0::shutdown();
        vu1::shutdown();
        iop::shutdown();
        ee::interrupt::shutdown();
        spu2::shutdown();
        memory::shutdown();
        ee::memory::shutdown();
        ee::cop0::shutdown();
        ee::cpu::shutdown();
        ee::vu0::shutdown();
        ee::vu1::shutdown();
        bios::shutdown();
        dma::shutdown();
        vif::shutdown();
        gif::shutdown();
        state::shutdown();
        memorycard::shutdown();
        cheat::shutdown();
        ee::dma::shutdown();
        ee::cpu::shutdownGPR();
        ee::cpu::shutdownPC();
        ee::memory::shutdownMainRAM();
        input::shutdown();
        cdvd::shutdown();
        gs::shutdown();

        initialized = false;
        LOGI("PS2 core shutdown complete");
    }

    void startLoop() {
        isRunning.store(true);
        std::thread loopThread(run);
        loopThread.detach();
    }

    bool isRunning() {
        return isRunning.load();
    }

    void emulationLoop() {
        LOGI("Iniciando loop principal de emulação...");

        const int frameTimeMs = 16; // ~60fps
        while (isRunning.load()) {
            if (isPaused.load()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(frameTimeMs));
                continue;
            }

            auto start = std::chrono::high_resolution_clock::now();

            LOGI("Executando ciclo de emulação...");

            // Update input states
            input::update();
            
            // Now you can query input states
            // Example:
            // if (input::getButtonState(ButtonCode::X)) { ... }
            // float lx = input::getAxisValue(AxisCode::AXIS_X);
            
            // Executa IOP antes dos outros processadores
            iop::runFrame();

            // Executa VU0 e VU1 antes do EE
            vu0::runFrame();
            vu1::runFrame();

            // Processa interrupções pendentes
            ee::interrupt::process();

            // Executa instruções do EE
            ee::step();

            // Processa DMA e outros subsistemas
            dma::step();
            vif::step();
            gif::step();

            // Processa áudio
            spu2::runFrame();

            ee::dma::simulateCycle();

            ee::memory::invalidateICache();
            ee::memory::invalidateDCache();

            // Debug GPR state if needed
            if (debugEnabled) {
                LOGI("GPR State:\n%s", ee::cpu::dumpGPR().c_str());
            }

            // Debug PC state if needed
            if (debugEnabled) {
                LOGI("PC: 0x%08X", ee::cpu::getPC());
            }

            // Debug memory state if needed
            if (debugEnabled) {
                LOGI("Memory State:\n%s", ee::memory::dumpMemory(0x00000000, 256).c_str());
            }

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start;

            if (elapsed.count() < frameTimeMs) {
                std::this_thread::sleep_for(std::chrono::milliseconds(frameTimeMs) - elapsed);
            }
        }

        LOGI("Loop principal finalizado.");
    }

    void step() {
        if (!initialized) return;

        // Update input states
        input::update();
        
        // Now you can query input states
        // Example:
        // if (input::getButtonState(ButtonCode::X)) { ... }
        // float lx = input::getAxisValue(AxisCode::AXIS_X);
        
        // Executa IOP antes dos outros processadores
        iop::runFrame();

        // Executa VU0 e VU1 antes do EE
        vu0::runFrame();
        vu1::runFrame();

        // Processa interrupções pendentes
        ee::interrupt::process();

        // Executa instruções do EE
        ee::step();

        // Processa DMA e outros subsistemas
        dma::step();
        vif::step();
        gif::step();

        // Processa áudio
        spu2::runFrame();

        ee::dma::simulateCycle();

        ee::memory::invalidateICache();
        ee::memory::invalidateDCache();

        // Debug GPR state if needed
        if (debugEnabled) {
            LOGI("GPR State:\n%s", ee::cpu::dumpGPR().c_str());
        }

        // Debug PC state if needed
        if (debugEnabled) {
            LOGI("PC: 0x%08X", ee::cpu::getPC());
        }

        // Debug memory state if needed
        if (debugEnabled) {
            LOGI("Memory State:\n%s", ee::memory::dumpMemory(0x00000000, 256).c_str());
        }
    }
}