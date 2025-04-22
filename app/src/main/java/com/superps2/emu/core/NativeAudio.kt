package com.superps2.emu.core

object NativeAudio {
    init { System.loadLibrary("superps2") }
    external fun initAudio(sampleRate: Int, channels: Int, bufferSizeInFrames: Int): Boolean
    external fun writeAudio(buffer: ShortArray, numSamples: Int)
    external fun shutdownAudio()
}