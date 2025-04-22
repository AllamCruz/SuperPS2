package com.superps2.emu

class EmulatorBridge {
    companion object {
        init {
            System.loadLibrary("superps2")
        }

        external fun saveState(savePath: String): Boolean
        external fun loadState(savePath: String): Boolean
        external fun stopEmulation(): Boolean
        external fun startEmulation(isoPath: String): Boolean
    }
} 