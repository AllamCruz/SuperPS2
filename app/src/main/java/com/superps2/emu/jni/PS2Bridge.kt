package com.superps2.emu.jni

object PS2Bridge {
    init {
        System.loadLibrary("superps2")
    }

    // Input control
    external fun sendButtonEvent(code: Int, pressed: Boolean)
    external fun onAxisEvent(axis: Int, value: Float)

    // Emulator control
    external fun startEmulation(isoPath: String): Boolean
    external fun pauseEmulation(): Boolean
    external fun resumeEmulation(): Boolean
    external fun resetEmulation(): Boolean
    external fun stopEmulation(): Boolean
    external fun isRunning(): Boolean
    external fun isPaused(): Boolean

    // State management
    external fun saveState(savePath: String): Boolean
    external fun loadState(savePath: String): Boolean

    // Audio control
    external fun initAudio(sampleRate: Int, channels: Int, bufferSize: Int): Boolean
    external fun writeAudio(data: ShortArray, numSamples: Int)
    external fun shutdownAudio()

    // OSD control
    external fun initOSD(targetFps: Double)
    external fun notifyFrame()
    external fun getFps(): Double
    external fun getSpeed(): Double
    external fun shutdownOSD()

    // Engine settings
    external fun setResolutionScale(scale: Float)
    external fun setFrameLimit(fps: Int)
    external fun setVSyncEnabled(enabled: Boolean)
    external fun setMultiThreadEnabled(enabled: Boolean)
    external fun setNeonOptimizations(enabled: Boolean)
} 