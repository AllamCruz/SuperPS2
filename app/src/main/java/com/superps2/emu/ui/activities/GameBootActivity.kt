package com.superps2.emu.ui.activities

import android.content.Intent
import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import com.superps2.emu.R
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch

class GameBootActivity : AppCompatActivity() {
    private lateinit var isoPath: String

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_game_boot)

        isoPath = intent.getStringExtra("ISO_PATH") ?: run {
            Toast.makeText(this, "Nenhuma ISO fornecida", Toast.LENGTH_LONG).show()
            finish()
            return
        }

        playBootAnimation()
    }

    private fun playBootAnimation() {
        // TODO: Implementar animações de boot
        lifecycleScope.launch {
            delay(3000L) // 3s de boot
            val intent = Intent(this@GameBootActivity, EmulationActivity::class.java).apply {
                putExtra("ISO_PATH", isoPath)
            }
            startActivity(intent)
            overridePendingTransition(android.R.anim.fade_in, android.R.anim.fade_out)
            finish()
        }
    }
}
