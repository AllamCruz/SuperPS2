package com.superps2.emu.ui.fragments

import android.app.Activity
import android.app.Activity.RESULT_OK
import android.content.Context
import android.content.Intent
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.provider.DocumentsContract
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.activity.result.contract.ActivityResultContracts
import androidx.cardview.widget.CardView
import androidx.fragment.app.Fragment
import com.superps2.emu.databinding.FragmentHomeBinding
import com.superps2.emu.R
import java.util.*
import androidx.recyclerview.widget.LinearLayoutManager
import com.superps2.emu.ui.adapters.ISOGameAdapter
import com.superps2.emu.core.ISOManager


class HomeFragment : Fragment() {

    private var _binding: FragmentHomeBinding? = null
    private val binding get() = _binding!!

    // Armazenador de resultado para seleção de pasta
    private val folderPickerLauncher =
        registerForActivityResult(ActivityResultContracts.StartActivityForResult()) { result ->
            if (result.resultCode == RESULT_OK) {
                val uri: Uri? = result.data?.data
                uri?.let {
                    requireContext().contentResolver.takePersistableUriPermission(
                        it, Intent.FLAG_GRANT_READ_URI_PERMISSION
                    )
                    // Salva o URI em SharedPreferences para uso posterior
                    saveSelectedFolder(it)
                    Toast.makeText(requireContext(), R.string.folder_selected, Toast.LENGTH_SHORT).show()
                }
            }
        }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        _binding = FragmentHomeBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        loadDeviceInfo()
        setupFolderPicker()
    }

    /**
     * Preenche os campos com informações do dispositivo atual
     */
    private fun loadDeviceInfo() {
        val manufacturer = Build.MANUFACTURER.capitalize(Locale.ROOT)
        val model = Build.MODEL
        val deviceName = "$manufacturer $model"
        val chipset = Build.HARDWARE.uppercase(Locale.ROOT)
        val totalRamMB = getTotalMemoryMB()

        val compatibility = when {
            totalRamMB >= 4000 -> getString(R.string.compatibility_high)
            totalRamMB >= 2500 -> getString(R.string.compatibility_medium)
            else -> getString(R.string.compatibility_low)
        }

        binding.textDeviceName.text = deviceName
        binding.textChipset.text = getString(R.string.device_chipset_format, chipset)
        binding.textRam.text = getString(R.string.device_ram_format, totalRamMB)
        binding.textCompatibility.text = getString(R.string.device_compatibility_format, compatibility)
    }

    /**
     * Inicia o seletor de pasta ao clicar no FAB
     */
    private fun setupFolderPicker() {
        binding.fabAddGame.setOnClickListener {
            val intent = Intent(Intent.ACTION_OPEN_DOCUMENT_TREE).apply {
                flags = Intent.FLAG_GRANT_READ_URI_PERMISSION or Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION
            }
            folderPickerLauncher.launch(intent)
        }
    }

    /**
     * Calcula a memória RAM total do dispositivo
     */
    private fun getTotalMemoryMB(): Int {
        val actManager = requireContext().getSystemService(Context.ACTIVITY_SERVICE) as android.app.ActivityManager
        val memInfo = android.app.ActivityManager.MemoryInfo()
        actManager.getMemoryInfo(memInfo)
        return (memInfo.totalMem / (1024 * 1024)).toInt()
    }

    /**
     * Salva o URI da pasta selecionada em SharedPreferences
     */
    private fun saveSelectedFolder(uri: Uri) {
        val prefs = requireContext().getSharedPreferences("superps2_prefs", Context.MODE_PRIVATE)
        prefs.edit().putString("isos_folder_uri", uri.toString()).apply()
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
    
    private fun setupGameList() {
        val games = ISOManager.scanISOFiles(requireContext())
    
        if (games.isEmpty()) {
            binding.textNoGamesFound.visibility = View.VISIBLE
            binding.recyclerViewGames.visibility = View.GONE
        } else {
            binding.textNoGamesFound.visibility = View.GONE
            binding.recyclerViewGames.apply {
                visibility = View.VISIBLE
                layoutManager = LinearLayoutManager(requireContext())
                adapter = ISOGameAdapter(games) { game ->
                    // Ação ao clicar em "Iniciar Jogo"
                    Toast.makeText(requireContext(), "Iniciando ${game.name}", Toast.LENGTH_SHORT).show()
                    // Em breve: chamada para iniciar o emulador
                }
            }
        }
    }
}
