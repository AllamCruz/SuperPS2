#ifndef SUPERPS2_INPUT_MANAGER_H
#define SUPERPS2_INPUT_MANAGER_H

#include <cstdint>

namespace input {

    // Maximum number of buttons/axes supported
    constexpr int MAX_BUTTONS = 16;
    constexpr int MAX_AXES    = 2;

    // Initialize input system
    bool init();

    // Set button state (pressed = true/false)
    void setButtonState(int buttonCode, bool pressed);

    // Set axis value (in [-1.0, +1.0])
    void setAxisValue(int axisCode, float value);

    // Update states (call once per frame before execInstruction)
    void update();

    // Query button state
    bool getButtonState(int buttonCode);

    // Query axis value
    float getAxisValue(int axisCode);

    // Clean up resources
    void shutdown();

} // namespace input

#endif // SUPERPS2_INPUT_MANAGER_H 