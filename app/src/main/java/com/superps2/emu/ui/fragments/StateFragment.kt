package com.superps2.emu.ui.fragments

import android.app.AlertDialog
import android.content.Intent
import android.os.Bundle
import android.view.*
import android.widget.Button
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import android.widget.Toast
import com.superps2.emu.R
import com.superps2.emu.state.StateManager
import com.superps2.emu.ui.adapters.StateAdapter
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

class StateFragment : Fragment() {
    private lateinit var btnSave: Button
    private lateinit var btnLoad: Button
    private lateinit var rvStates: RecyclerView
    private lateinit var adapter: StateAdapter
    private val states = mutableListOf<String>()

    override fun onCreateView(inflater: LayoutInflater, c: ViewGroup?, s: Bundle?) =
        inflater.inflate(R.layout.fragment_states, c, false)

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        btnSave = view.findViewById(R.id.btnSaveState)
        btnLoad = view.findViewById(R.id.btnLoadState)
        rvStates = view.findViewById(R.id.rvStates)

        adapter = StateAdapter(states)
        rvStates.layoutManager = LinearLayoutManager(context)
        rvStates.adapter = adapter

        loadStateList()

        btnSave.setOnClickListener {
            // Gera nome baseado em timestamp
            val name = "state_${System.currentTimeMillis()}.sps2"
            CoroutineScope(Dispatchers.IO).launch {
                val ok = StateManager.save(requireContext(), /* path: */ context!!.filesDir.absolutePath + "/states/$name")
                CoroutineScope(Dispatchers.Main).launch {
                    if (ok) {
                        Toast.makeText(context, "Saved $name", Toast.LENGTH_SHORT).show()
                        loadStateList()
                    } else Toast.makeText(context, "Save failed", Toast.LENGTH_SHORT).show()
                }
            }
        }

        btnLoad.setOnClickListener {
            val pos = adapter.selectedPosition
            if (pos < 0 || pos >= states.size) {
                Toast.makeText(context, "Select a state", Toast.LENGTH_SHORT).show()
                return@setOnClickListener
            }
            val name = states[pos]
            CoroutineScope(Dispatchers.IO).launch {
                val ok = StateManager.load(requireContext(), context!!.filesDir.absolutePath + "/states/$name")
                CoroutineScope(Dispatchers.Main).launch {
                    if (ok) Toast.makeText(context, "Loaded $name", Toast.LENGTH_SHORT).show()
                    else Toast.makeText(context, "Load failed", Toast.LENGTH_SHORT).show()
                }
            }
        }
    }

    private fun loadStateList() {
        val dir = context!!.filesDir.resolve("states")
        if (!dir.exists()) dir.mkdirs()
        states.clear()
        dir.listFiles()?.filter { it.extension == "sps2" }?.forEach {
            states.add(it.name)
        }
        adapter.notifyDataSetChanged()
    }
} 