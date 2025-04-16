package com.superps2.emu.core

object NativeBridge {
    init {
        System.loadLibrary("superps2") // Nome do .so gerado pelo C++
    }

    external fun startEmulation(isoPath: String): Boolean
}
