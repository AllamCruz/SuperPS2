package com.superps2.emu.core

object PS2Bridge {
    init {
        System.loadLibrary("superps2")
    }

    external fun loadISO(path: String): Boolean
    external fun runEmulator()
    external fun pauseEmulator()
    external fun resumeEmulator()
    external fun stopEmulator()
} 