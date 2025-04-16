package com.superps2.emu.ui.activities

import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.superps2.emu.R
import com.superps2.emu.core.NativeBridge

class EmulationActivity : AppCompatActivity() {

    companion object {
        const val EXTRA_ISO_PATH = "ISO_PATH"
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_emulation)

        val isoPath = intent.getStringExtra(EXTRA_ISO_PATH)

        if (isoPath.isNullOrEmpty()) {
            Toast.makeText(this, getString(R.string.invalid_iso_path), Toast.LENGTH_SHORT).show()
            finish()
            return
        }

        val success = try {
            NativeBridge.startEmulation(isoPath)
        } catch (e: Exception) {
            Log.e("EmulationActivity", "Erro ao iniciar JNI: ${e.message}")
            false
        }

        if (!success) {
            Toast.makeText(this, getString(R.string.emulation_failed), Toast.LENGTH_SHORT).show()
            finish()
        } else {
            findViewById<View>(R.id.loadingOverlay).visibility = View.GONE
            // Futuramente: adicionar SurfaceView/GLSurfaceView aqui
        }
    }
}
