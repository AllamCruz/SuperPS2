package com.superps2.emu.ui.activities

import android.os.Bundle
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import com.superps2.emu.R
import com.superps2.emu.audio.AudioManager
import com.superps2.emu.core.PS2Bridge
import com.superps2.emu.core.NativeBridge
import com.superps2.emu.databinding.ActivityEmulationBinding
import com.superps2.emu.ui.views.EmulationOverlayView
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class EmulationActivity : AppCompatActivity() {
    private lateinit var binding: ActivityEmulationBinding
    private lateinit var audioManager: AudioManager
    private var isoPath: String = ""
    private var isEmulationRunning = false
    private lateinit var emulatorSurface: SurfaceView
    private lateinit var overlay: EmulationOverlayView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityEmulationBinding.inflate(layoutInflater)
        setContentView(binding.root)
        overridePendingTransition(android.R.anim.fade_in, android.R.anim.fade_out)

        isoPath = intent.getStringExtra("iso_path") ?: run {
            Toast.makeText(this, "No ISO path provided", Toast.LENGTH_LONG).show()
            finish()
            return
        }

        // Initialize audio with 44.1 kHz, stereo, 2048 frame buffer
        audioManager = AudioManager()
        audioManager.start(sampleRate = 44100, channels = 2, bufferSize = 2048)

        // Initialize surface view
        emulatorSurface = binding.emulatorSurface
        emulatorSurface.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceCreated(holder: SurfaceHolder) {
                startEmulation()
            }

            override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
                // Handle surface changes if needed
            }

            override fun surfaceDestroyed(holder: SurfaceHolder) {
                stopEmulation()
            }
        })

        // Initialize overlay view
        overlay = binding.emulationOverlay
        overlay.isClickable = true
        overlay.isFocusable = true
        
        // Set up overlay visibility based on settings
        overlay.visibility = if (settings.showOverlay) View.VISIBLE else View.GONE
    }

    private fun startEmulation() {
        val loadingOverlay = binding.loadingOverlay
        loadingOverlay.alpha = 0f
        loadingOverlay.animate().alpha(1f).setDuration(500).start()

        lifecycleScope.launch(Dispatchers.IO) {
            try {
                if (!PS2Bridge.loadISO(isoPath)) {
                    withContext(Dispatchers.Main) {
                        Toast.makeText(this@EmulationActivity, "Failed to load ISO", Toast.LENGTH_LONG).show()
                        finish()
                    }
                    return@launch
                }

                isEmulationRunning = true
                PS2Bridge.runEmulator()

                withContext(Dispatchers.Main) {
                    loadingOverlay.animate().alpha(0f).setDuration(300).withEndAction {
                        loadingOverlay.visibility = View.GONE
                    }.start()
                }
            } catch (e: Exception) {
                withContext(Dispatchers.Main) {
                    Toast.makeText(this@EmulationActivity, "Erro na emulação: ${e.message}", Toast.LENGTH_LONG).show()
                    finish()
                }
            }
        }
    }

    private fun stopEmulation() {
        if (isEmulationRunning) {
            isEmulationRunning = false
            PS2Bridge.stopEmulator()
        }
        audioManager.stop()
    }

    override fun onPause() {
        super.onPause()
        if (isEmulationRunning) {
            PS2Bridge.pauseEmulator()
        }
        audioManager.stop()
    }

    override fun onResume() {
        super.onResume()
        if (isEmulationRunning) {
            PS2Bridge.resumeEmulator()
        }
        audioManager.start(sampleRate = 44100, channels = 2, bufferSize = 2048)
    }

    override fun onDestroy() {
        super.onDestroy()
        stopEmulation()
        audioManager.stop()
    }

    companion object {
        // Settings object to manage overlay visibility
        private val settings = object {
            var showOverlay = true
        }
    }
}
