package com.superps2.emu.ui.fragments

import android.content.Context
import android.content.SharedPreferences
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import com.superps2.emu.databinding.FragmentSettingsBinding
import com.google.android.material.switchmaterial.SwitchMaterial
import android.widget.ArrayAdapter
import android.widget.SeekBar
import android.widget.Spinner
import android.widget.Toast

class SettingsFragment : Fragment() {

    private var _binding: FragmentSettingsBinding? = null
    private val binding get() = _binding!!

    private lateinit var preferences: SharedPreferences

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        _binding = FragmentSettingsBinding.inflate(inflater, container, false)
        preferences = requireContext().getSharedPreferences("SuperPS2Settings", Context.MODE_PRIVATE)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        setupGraphicsSettings()
        setupAudioSettings()
        setupControlSettings()
        setupPerformanceSettings()
        setupAdvancedSettings()
    }

    // -------------------- Gráficos --------------------
    private fun setupGraphicsSettings() {
        val resOptions = arrayOf("1x", "2x", "3x", "4x")
        val adapter = ArrayAdapter(requireContext(), android.R.layout.simple_spinner_item, resOptions)
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
        binding.spinnerResolution.adapter = adapter

        val savedRes = preferences.getString("graphics_resolution", "1x")
        binding.spinnerResolution.setSelection(resOptions.indexOf(savedRes))

        binding.spinnerResolution.setOnItemSelectedListener { _, _, position, _ ->
            preferences.edit().putString("graphics_resolution", resOptions[position]).apply()
        }

        binding.switchAnisotropic.isChecked = preferences.getBoolean("graphics_anisotropic", false)
        binding.switchAnisotropic.setOnCheckedChangeListener { _, isChecked ->
            preferences.edit().putBoolean("graphics_anisotropic", isChecked).apply()
        }

        binding.switchUpscaling.isChecked = preferences.getBoolean("graphics_upscaling", false)
        binding.switchUpscaling.setOnCheckedChangeListener { _, isChecked ->
            preferences.edit().putBoolean("graphics_upscaling", isChecked).apply()
        }
    }

    // -------------------- Áudio --------------------
    private fun setupAudioSettings() {
        val audioQualities = arrayOf("Baixa", "Média", "Alta")
        val adapter = ArrayAdapter(requireContext(), android.R.layout.simple_spinner_item, audioQualities)
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
        binding.spinnerAudioQuality.adapter = adapter

        val savedQuality = preferences.getString("audio_quality", "Média")
        binding.spinnerAudioQuality.setSelection(audioQualities.indexOf(savedQuality))

        binding.spinnerAudioQuality.setOnItemSelectedListener { _, _, position, _ ->
            preferences.edit().putString("audio_quality", audioQualities[position]).apply()
        }

        binding.switchAudio.isChecked = preferences.getBoolean("audio_enabled", true)
        binding.switchAudio.setOnCheckedChangeListener { _, isChecked ->
            preferences.edit().putBoolean("audio_enabled", isChecked).apply()
        }
    }

    // -------------------- Controles --------------------
    private fun setupControlSettings() {
        val layouts = arrayOf("Padrão", "Alternativo")
        val adapter = ArrayAdapter(requireContext(), android.R.layout.simple_spinner_item, layouts)
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
        binding.spinnerControlLayout.adapter = adapter

        val savedLayout = preferences.getString("control_layout", "Padrão")
        binding.spinnerControlLayout.setSelection(layouts.indexOf(savedLayout))

        binding.spinnerControlLayout.setOnItemSelectedListener { _, _, position, _ ->
            preferences.edit().putString("control_layout", layouts[position]).apply()
        }

        binding.switchVibration.isChecked = preferences.getBoolean("control_vibration", false)
        binding.switchVibration.setOnCheckedChangeListener { _, isChecked ->
            preferences.edit().putBoolean("control_vibration", isChecked).apply()
        }
    }

    // -------------------- Desempenho --------------------
    private fun setupPerformanceSettings() {
        val savedFps = preferences.getInt("performance_fps_limit", 60)
        binding.seekbarFps.progress = savedFps
        binding.txtFpsValue.text = "$savedFps FPS"

        binding.seekbarFps.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, value: Int, fromUser: Boolean) {
                binding.txtFpsValue.text = "$value FPS"
                preferences.edit().putInt("performance_fps_limit", value).apply()
            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {}
            override fun onStopTrackingTouch(seekBar: SeekBar?) {}
        })

        binding.switchSnapdragon.isChecked = preferences.getBoolean("performance_snapdragon", false)
        binding.switchSnapdragon.setOnCheckedChangeListener { _, isChecked ->
            preferences.edit().putBoolean("performance_snapdragon", isChecked).apply()
        }
    }

    // -------------------- Avançado --------------------
    private fun setupAdvancedSettings() {
        binding.switchShowLogs.isChecked = preferences.getBoolean("advanced_logs", false)
        binding.switchShowLogs.setOnCheckedChangeListener { _, isChecked ->
            preferences.edit().putBoolean("advanced_logs", isChecked).apply()
        }

        binding.switchCompatMode.isChecked = preferences.getBoolean("advanced_compat", false)
        binding.switchCompatMode.setOnCheckedChangeListener { _, isChecked ->
            preferences.edit().putBoolean("advanced_compat", isChecked).apply()
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}
