package com.superps2.emu.adapter

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.superps2.emu.R
import com.superps2.emu.model.GameFile
import java.text.SimpleDateFormat
import java.util.*

class GameAdapter(
    private var gameList: List<GameFile>,
    private val onStartClick: (GameFile) -> Unit
) : RecyclerView.Adapter<GameAdapter.GameViewHolder>() {

    class GameViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val tvGameTitle: TextView = view.findViewById(R.id.tvGameTitle)
        val tvGameSize: TextView = view.findViewById(R.id.tvGameSize)
        val tvGameDate: TextView = view.findViewById(R.id.tvGameDate)
        val btnStartGame: Button = view.findViewById(R.id.btnStartGame)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): GameViewHolder {
        val view = LayoutInflater.from(parent.context)
            .inflate(R.layout.item_game, parent, false)
        return GameViewHolder(view)
    }

    override fun onBindViewHolder(holder: GameViewHolder, position: Int) {
        val game = gameList[position]
        holder.tvGameTitle.text = game.name
        holder.tvGameSize.text = "Tamanho: ${game.sizeInMB} MB"

        val date = Date(game.lastModified)
        val formattedDate = SimpleDateFormat("dd/MM/yyyy", Locale.getDefault()).format(date)
        holder.tvGameDate.text = "Modificado: $formattedDate"

        holder.btnStartGame.setOnClickListener {
            onStartClick(game)
        }
    }

    override fun getItemCount() = gameList.size

    fun updateList(newList: List<GameFile>) {
        gameList = newList
        notifyDataSetChanged()
    }
} 