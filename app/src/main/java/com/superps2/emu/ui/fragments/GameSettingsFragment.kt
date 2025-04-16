package com.superps2.emu.ui.fragments

import android.content.Context
import android.net.Uri
import android.os.Bundle
import android.util.Log
import android.view.*
import android.widget.*
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.navArgs
import com.superps2.emu.databinding.FragmentGameSettingsBinding
import java.security.MessageDigest

class GameSettingsFragment : Fragment() {

    private var _binding: FragmentGameSettingsBinding? = null
    private val binding get() = _binding!!

    private val args: GameSettingsFragmentArgs by navArgs() // SafeArgs
    private lateinit var isoUri: String
    private lateinit var prefsKey: String

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        _binding = FragmentGameSettingsBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        isoUri = args.isoUri
        prefsKey = "config_${hashUri(isoUri)}"

        loadSettings()

        binding.btnSave.setOnClickListener { saveSettings() }
        binding.btnReset.setOnClickListener { resetSettings() }
    }

    // Carrega as configurações do jogo específico
    private fun loadSettings() {
        val prefs = requireContext().getSharedPreferences(prefsKey, Context.MODE_PRIVATE)

        binding.spinnerResolution.setSelection(prefs.getInt("resolution", 0))
        binding.switchAA.isChecked = prefs.getBoolean("aa", false)
        binding.spinnerRenderer.setSelection(prefs.getInt("renderer", 0))
        binding.switchAudio.isChecked = prefs.getBoolean("audio", true)
        binding.spinnerSync.setSelection(prefs.getInt("sync", 0))
        binding.seekbarCpuSpeed.progress = prefs.getInt("cpu_speed", 100)
        binding.switchFrameSkip.isChecked = prefs.getBoolean("frame_skip", false)
    }

    // Salva as configurações personalizadas para a ISO
    private fun saveSettings() {
        val prefs = requireContext().getSharedPreferences(prefsKey, Context.MODE_PRIVATE).edit()

        prefs.putInt("resolution", binding.spinnerResolution.selectedItemPosition)
        prefs.putBoolean("aa", binding.switchAA.isChecked)
        prefs.putInt("renderer", binding.spinnerRenderer.selectedItemPosition)
        prefs.putBoolean("audio", binding.switchAudio.isChecked)
        prefs.putInt("sync", binding.spinnerSync.selectedItemPosition)
        prefs.putInt("cpu_speed", binding.seekbarCpuSpeed.progress)
        prefs.putBoolean("frame_skip", binding.switchFrameSkip.isChecked)

        prefs.apply()
        Toast.makeText(requireContext(), R.string.settings_saved, Toast.LENGTH_SHORT).show()
    }

    // Restaura para configurações globais removendo prefs do jogo
    private fun resetSettings() {
        requireContext().getSharedPreferences(prefsKey, Context.MODE_PRIVATE).edit().clear().apply()
        loadSettings()
        Toast.makeText(requireContext(), R.string.settings_reset, Toast.LENGTH_SHORT).show()
    }

    // Cria hash simples do URI (MD5)
    private fun hashUri(uri: String): String {
        val md = MessageDigest.getInstance("MD5")
        val bytes = md.digest(uri.toByteArray())
        return bytes.joinToString("") { "%02x".format(it) }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}
