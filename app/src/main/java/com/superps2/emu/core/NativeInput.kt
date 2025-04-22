package com.superps2.emu.core

object NativeInput {
    init {
        System.loadLibrary("superps2")
    }

    external fun onButtonEvent(buttonCode: Int, pressed: Boolean)
    external fun onAxisEvent(axisCode: Int, value: Float)
    external fun getControllerState(): Long // pointer to ControllerState
}
