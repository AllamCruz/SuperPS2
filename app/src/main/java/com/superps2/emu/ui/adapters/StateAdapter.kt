package com.superps2.emu.ui.adapters

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView

class StateAdapter(private val items: List<String>) :
    RecyclerView.Adapter<StateAdapter.VH>() {
    var selectedPosition = -1

    inner class VH(v: View) : RecyclerView.ViewHolder(v) {
        val tv = v.findViewById<TextView>(android.R.id.text1)
        init {
            v.setOnClickListener {
                val prev = selectedPosition
                selectedPosition = adapterPosition
                notifyItemChanged(prev)
                notifyItemChanged(selectedPosition)
            }
        }
    }

    override fun onCreateViewHolder(p: ViewGroup, v: Int) =
        VH(LayoutInflater.from(p.context).inflate(android.R.layout.simple_list_item_single_choice, p, false))

    override fun getItemCount() = items.size

    override fun onBindViewHolder(h: VH, pos: Int) {
        h.tv.text = items[pos]
        h.itemView.isSelected = (pos == selectedPosition)
    }
} 