package com.superps2.emu.ui.fragments

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ArrayAdapter
import android.widget.Toast
import androidx.fragment.app.Fragment
import com.superps2.emu.R
import com.superps2.emu.memorycard.MemoryCardManager
import kotlinx.android.synthetic.main.fragment_memory_card.*

class MemoryCardFragment : Fragment() {
    private var currentSlot = 0
    private var currentSave: String? = null

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_memory_card, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        // Inicializa o spinner de slots
        val slots = arrayOf("Slot 1", "Slot 2")
        spinnerSlot.adapter = ArrayAdapter(requireContext(), android.R.layout.simple_spinner_item, slots)

        // Configura botão de abrir cartão
        btnOpenCard.setOnClickListener {
            currentSlot = spinnerSlot.selectedItemPosition
            if (MemoryCardManager.open(currentSlot)) {
                updateSavesList()
                Toast.makeText(context, "Cartão aberto com sucesso", Toast.LENGTH_SHORT).show()
            } else {
                Toast.makeText(context, "Falha ao abrir cartão", Toast.LENGTH_SHORT).show()
            }
        }

        // Configura lista de saves
        listSaves.setOnItemClickListener { _, _, position, _ ->
            currentSave = listSaves.adapter.getItem(position) as String
            Toast.makeText(context, "Save selecionado: $currentSave", Toast.LENGTH_SHORT).show()
        }

        // Configura botão de salvar bloco
        btnSaveState.setOnClickListener {
            if (currentSave == null) {
                Toast.makeText(context, "Selecione um save primeiro", Toast.LENGTH_SHORT).show()
                return@setOnClickListener
            }

            // Aqui você implementaria a lógica para salvar um bloco
            // Por exemplo, usando um FilePicker para selecionar o arquivo
            Toast.makeText(context, "Funcionalidade de salvar bloco em desenvolvimento", Toast.LENGTH_SHORT).show()
        }
    }

    private fun updateSavesList() {
        val saves = MemoryCardManager.getSaves()
        listSaves.adapter = ArrayAdapter(requireContext(), android.R.layout.simple_list_item_1, saves)
    }
} 