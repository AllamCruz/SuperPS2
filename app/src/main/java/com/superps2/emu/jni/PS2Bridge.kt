package com.superps2.emu.jni

object PS2Bridge {
    init {
        System.loadLibrary("superps2")
    }

    // Input control
    external fun sendButtonEvent(code: Int, pressed: Boolean)

    // Emulator control
    external fun startGame(isoPath: String): Boolean
    external fun pauseGame()
    external fun resumeGame()
    external fun resetGame()
    external fun stopGame()
    external fun isRunning(): Boolean
    external fun isPaused(): Boolean
} 