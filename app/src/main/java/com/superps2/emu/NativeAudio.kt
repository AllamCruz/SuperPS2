package com.superps2.emu

class NativeAudio {
    companion object {
        init {
            System.loadLibrary("superps2")
        }

        external fun initAudio(sampleRate: Int, channels: Int, bufferSize: Int)
        external fun writeAudio(buffer: ByteArray, size: Int)
        external fun shutdownAudio()
    }
} 