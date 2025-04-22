package com.superps2.emu.ui

import android.content.Intent
import android.os.Bundle
import android.widget.ProgressBar
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import com.superps2.emu.jni.PS2Bridge
import com.superps2.R
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class LoadingActivity : AppCompatActivity() {
    private lateinit var progressBar: ProgressBar
    private lateinit var tvStatus: TextView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_loading)

        progressBar = findViewById(R.id.progressBar)
        tvStatus = findViewById(R.id.tvStatus)

        // Recebe o path da ISO
        val isoPath = intent.getStringExtra("ISO_PATH")
        if (isoPath.isNullOrEmpty()) {
            Toast.makeText(this, "Nenhuma ISO fornecida", Toast.LENGTH_LONG).show()
            finish(); return
        }

        // Inicia o carregamento em background
        lifecycleScope.launch(Dispatchers.IO) {
            // Simular progresso incremental enquanto chama loadISO
            val loadJob = launch {
                val ok = PS2Bridge.startGame(isoPath)
                withContext(Dispatchers.Main) {
                    if (!ok) {
                        Toast.makeText(this@LoadingActivity, "Falha ao carregar ISO", Toast.LENGTH_LONG).show()
                        finish()
                    }
                }
            }

            // Enquanto carrega, animar a barra
            var progress = 0
            while (loadJob.isActive) {
                delay(100L)
                progress = (progress + 5).coerceAtMost(95)
                withContext(Dispatchers.Main) {
                    progressBar.progress = progress
                    tvStatus.text = "Carregando ISO... $progress%"
                }
            }
            // Aguarda término
            loadJob.join()
            // Finaliza progresso
            withContext(Dispatchers.Main) {
                progressBar.progress = 100
                tvStatus.text = "ISO carregada!"
            }
            delay(200L) // breve pausa para UX

            // Inicia GameBootActivity
            startActivity(Intent(this@LoadingActivity, GameBootActivity::class.java).apply {
                putExtra("ISO_PATH", isoPath)
            })
            overridePendingTransition(android.R.anim.fade_in, android.R.anim.fade_out)
            finish()
        }
    }
} 