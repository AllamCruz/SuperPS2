package com.superps2.emu.osd

import android.os.Handler
import android.os.Looper
import android.view.View
import android.widget.TextView
import com.superps2.emu.core.OSDBridge

class OSDManager(
    private val rootView: View,
    private val tvFps: TextView,
    private val tvSpeed: TextView
) {
    private val handler = Handler(Looper.getMainLooper())
    private val updateRunnable = object : Runnable {
        override fun run() {
            tvFps.text = String.format("FPS: %.1f", OSDBridge.getFps())
            tvSpeed.text = String.format("Speed: %.0f%%", OSDBridge.getSpeed())
            handler.postDelayed(this, 500)
        }
    }

    fun start(targetFps: Double = 60.0) {
        OSDBridge.initOSD(targetFps)
        handler.post(updateRunnable)
    }

    fun frameRendered() {
        OSDBridge.notifyFrame()
    }

    fun stop() {
        handler.removeCallbacks(updateRunnable)
        OSDBridge.shutdownOSD()
    }

    fun setVisible(visible: Boolean) {
        rootView.visibility = if (visible) View.VISIBLE else View.GONE
    }
} 