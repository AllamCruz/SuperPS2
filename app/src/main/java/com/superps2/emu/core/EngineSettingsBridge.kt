package com.superps2.emu.core

object EngineSettingsBridge {
    init { System.loadLibrary("superps2") }
    external fun setResolutionScale(scale: Float)
    external fun setFrameLimit(fps: Int)
    external fun setVSyncEnabled(enabled: Boolean)
    external fun setMultiThreadEnabled(enabled: Boolean)
    external fun setNeonOptimizations(enabled: Boolean)
} 