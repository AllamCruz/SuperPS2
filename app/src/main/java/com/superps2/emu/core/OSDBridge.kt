package com.superps2.emu.core

object OSDBridge {
    init { System.loadLibrary("superps2") }
    external fun initOSD(targetFps: Double)
    external fun notifyFrame()
    external fun getFps(): Double
    external fun getSpeed(): Double
    external fun shutdownOSD()
} 