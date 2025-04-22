package com.superps2.emu.ui

import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.google.android.material.button.MaterialButton
import com.google.android.material.switchmaterial.SwitchMaterial
import com.superps2.emu.R
import com.superps2.emu.bridge.EmulatorBridge
import java.io.File

class SettingsActivity : AppCompatActivity() {
    private lateinit var btnSaveState: MaterialButton
    private lateinit var btnLoadState: MaterialButton
    private lateinit var switchTouchControls: SwitchMaterial
    private lateinit var switchVibration: SwitchMaterial

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_settings)

        btnSaveState = findViewById(R.id.btnSaveState)
        btnLoadState = findViewById(R.id.btnLoadState)
        switchTouchControls = findViewById(R.id.switchTouchControls)
        switchVibration = findViewById(R.id.switchVibration)

        setupButtons()
        loadSettings()
    }

    private fun setupButtons() {
        btnSaveState.setOnClickListener {
            val savePath = File(filesDir, "savestate.sav").absolutePath
            if (EmulatorBridge.saveState(savePath)) {
                Toast.makeText(this, R.string.state_saved, Toast.LENGTH_SHORT).show()
            } else {
                Toast.makeText(this, R.string.error_saving_state, Toast.LENGTH_SHORT).show()
            }
        }

        btnLoadState.setOnClickListener {
            val savePath = File(filesDir, "savestate.sav").absolutePath
            if (EmulatorBridge.loadState(savePath)) {
                Toast.makeText(this, R.string.state_loaded, Toast.LENGTH_SHORT).show()
            } else {
                Toast.makeText(this, R.string.error_loading_state, Toast.LENGTH_SHORT).show()
            }
        }

        switchTouchControls.setOnCheckedChangeListener { _, isChecked ->
            // TODO: Implementar controle de toque
        }

        switchVibration.setOnCheckedChangeListener { _, isChecked ->
            // TODO: Implementar vibração
        }
    }

    private fun loadSettings() {
        // TODO: Carregar configurações salvas
    }

    override fun onPause() {
        super.onPause()
        // TODO: Salvar configurações
    }
} 