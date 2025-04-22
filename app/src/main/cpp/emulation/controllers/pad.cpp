#include "pad.h"
#include "../input/input_manager.h"
#include <array>
#include <android/log.h>

#define TAG "Pad"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

namespace pad {
    // Cada port tem 16‑bit de botões + 2 eixos de 8‑bit cada
    struct PadState {
        uint16_t buttons; // bit0 = Cross, bit1 = Circle, etc
        int8_t  lx, ly;   // eixo X, Y [-128..127]
        int8_t  rx, ry;   // segundo stick (se quiser)
    };

    static std::array<PadState, 2> ports;

    bool init() {
        input::init();
        for (auto &p : ports) {
            p = { 0xFFFF, 0, 0, 0, 0 };
        }
        LOGI("Pad subsystem initialized.");
        return true;
    }

    void shutdown() {
        input::shutdown();
        LOGI("Pad subsystem shutdown.");
    }

    void update() {
        input::update();
        // Mapear InputManager para PadState
        for (int port = 0; port < 2; ++port) {
            uint16_t b = 0xFFFF;
            if (input::getButtonState(4))  b &= ~(1<<0); // X
            if (input::getButtonState(5))  b &= ~(1<<1); // Circle
            if (input::getButtonState(6))  b &= ~(1<<2); // Triangle
            if (input::getButtonState(7))  b &= ~(1<<3); // Square
            if (input::getButtonState(0))  b &= ~(1<<4); // Up
            if (input::getButtonState(1))  b &= ~(1<<5); // Down
            if (input::getButtonState(2))  b &= ~(1<<6); // Left
            if (input::getButtonState(3))  b &= ~(1<<7); // Right
            // Start/Select
            if (input::getButtonState(8))  b &= ~(1<<8);
            if (input::getButtonState(9))  b &= ~(1<<9);
            ports[port].buttons = b;
            // Eixos normalizados [-1,1] → [-128,127]
            float fx = input::getAxisValue(0);
            float fy = input::getAxisValue(1);
            ports[port].lx = static_cast<int8_t>(fx * 127);
            ports[port].ly = static_cast<int8_t>(fy * 127);
        }
    }

    uint32_t readRegister(int port, uint32_t regOffset) {
        // Endereços típicos: 0x1F801040 + port*0x10 + regOffset
        auto &st = ports[port];
        switch (regOffset) {
            case 0x00: // BUTTON data low
                return st.buttons & 0xFF;
            case 0x04: // BUTTON data high
                return (st.buttons >> 8) & 0xFF;
            case 0x08: // LX
                return static_cast<uint8_t>(st.lx);
            case 0x0C: // LY
                return static_cast<uint8_t>(st.ly);
            default:
                return 0;
        }
    }

    void writeRegister(int, uint32_t, uint32_t) {
        // Geralmente não usado
    }
} 