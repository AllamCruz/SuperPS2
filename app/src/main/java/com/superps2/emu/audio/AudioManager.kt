package com.superps2.emu.audio

import android.util.Log
import com.superps2.emu.NativeAudio
import kotlinx.coroutines.*
import java.util.concurrent.LinkedBlockingQueue

class AudioManager {
    private val bufferQueue = LinkedBlockingQueue<ShortArray>()
    private var job: Job? = null
    private var isRunning = false
    private var sampleRate = 0
    private var channels = 0
    private var bufferSize = 0

    fun start(sampleRate: Int, channels: Int, bufferSize: Int) {
        if (isRunning) return

        this.sampleRate = sampleRate
        this.channels = channels
        this.bufferSize = bufferSize

        try {
            NativeAudio.initAudio(sampleRate, channels, bufferSize)
            isRunning = true
            Log.d("AudioManager", "Audio initialized: $sampleRate Hz, $channels channels, $bufferSize frames")
            job = CoroutineScope(Dispatchers.IO).launch {
                while (isActive) {
                    val buffer = bufferQueue.take()
                    NativeAudio.writeAudio(buffer, buffer.size)
                }
            }
        } catch (e: Exception) {
            Log.e("AudioManager", "Failed to initialize audio: ${e.message}")
        }
    }

    fun feed(pcm: ShortArray) {
        bufferQueue.offer(pcm)
    }

    fun stop() {
        if (!isRunning) return

        try {
            job?.cancel()
            NativeAudio.shutdownAudio()
            isRunning = false
            Log.d("AudioManager", "Audio shutdown")
        } catch (e: Exception) {
            Log.e("AudioManager", "Failed to shutdown audio: ${e.message}")
        }
    }

    fun writeAudio(buffer: ByteArray, size: Int) {
        if (!isRunning) return

        try {
            NativeAudio.writeAudio(buffer, size)
        } catch (e: Exception) {
            Log.e("AudioManager", "Failed to write audio: ${e.message}")
        }
    }
}