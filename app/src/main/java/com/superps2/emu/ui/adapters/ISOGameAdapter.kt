package com.superps2.emu.ui.adapters

import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView
import com.superps2.emu.databinding.ItemIsoGameBinding
import com.superps2.emu.core.ISOGame

class ISOGameAdapter(
    private val games: List<ISOGame>,
    private val onPlayClick: (ISOGame) -> Unit
    private val context: Context,
    private val isoList: List<ISOGame>,
    private val onStartGame: (ISOGame) -> Unit,
    private val onConfigureGame: (ISOGame) -> Unit
) : RecyclerView.Adapter<ISOGameAdapter.ViewHolder>() {

    inner class ViewHolder(val binding: ItemIsoGameBinding) : RecyclerView.ViewHolder(binding.root)
    val txtGameTitle: TextView = itemView.findViewById(R.id.txtGameTitle)
    val txtGameSize: TextView = itemView.findViewById(R.id.txtGameSize)
    val btnStartGame: Button = itemView.findViewById(R.id.btnStartGame)
    val btnConfig: ImageButton = itemView.findViewById(R.id.btnConfig)

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val binding = ItemIsoGameBinding.inflate(LayoutInflater.from(parent.context), parent, false)
        return ViewHolder(binding)
        val view = LayoutInflater.from(parent.context).inflate(R.layout.item_iso_game, parent, false)
        return ISOViewHolder(view)
    }

    override fun getItemCount(): Int = games.size

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val game = games[position]
        val isoGame = isoList[position]
        holder.txtGameTitle.text = isoGame.name
        holder.txtGameSize.text = isoGame.getFormattedSize()
        holder.btnStartGame.setOnClickListener {
            val intent = Intent(context, EmulationActivity::class.java)
            intent.putExtra(EmulationActivity.EXTRA_ISO_PATH, isoGame.uri)
            context.startActivity(intent)
        with(holder.binding) {
            textGameName.text = game.name
            textGameSize.text = formatSize(game.sizeInMB)
            holder.btnStartGame.setOnClickListener {
                onStartGame(isoGame)
                holder.btnConfig.setOnClickListener {
                    onConfigureGame(isoGame)

            // Clique no botão de iniciar
            btnPlay.setOnClickListener {
                onPlayClick(game)
            }
        }
    }
    override fun getItemCount(): Int = isoList.size

    private fun formatSize(sizeInMB: Float): String {
        return if (sizeInMB >= 1024f) {
            String.format("%.2f GB", sizeInMB / 1024f)
        } else {
            String.format("%.1f MB", sizeInMB)
        }
    }
    onBindViewHolder(holder: ViewHolder, position: Int)
    
}