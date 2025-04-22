package com.superps2.emu.core

class InputBridge {
    companion object {
        init {
            System.loadLibrary("superps2")
        }

        // PS2 Button Constants
        const val PS2_BUTTON_UP = 0
        const val PS2_BUTTON_DOWN = 1
        const val PS2_BUTTON_LEFT = 2
        const val PS2_BUTTON_RIGHT = 3
        const val PS2_BUTTON_CIRCLE = 4
        const val PS2_BUTTON_X = 5
        const val PS2_BUTTON_TRIANGLE = 6
        const val PS2_BUTTON_SQUARE = 7
        const val PS2_BUTTON_START = 8
        const val PS2_BUTTON_SELECT = 9
        const val PS2_BUTTON_L1 = 10
        const val PS2_BUTTON_R1 = 11
        const val PS2_BUTTON_L2 = 12
        const val PS2_BUTTON_R2 = 13
        const val PS2_BUTTON_L3 = 14
        const val PS2_BUTTON_R3 = 15

        external fun setButtonState(button: Int, pressed: Boolean)
        external fun setAnalogStick(stick: Int, x: Float, y: Float)
    }
} 