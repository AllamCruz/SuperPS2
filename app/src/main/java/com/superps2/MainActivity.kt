package com.superps2

import android.Manifest
import android.content.pm.PackageManager
import android.os.Bundle
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.superps2.emu.EmulatorBridge
import java.io.File

class MainActivity : AppCompatActivity() {
    private var currentIsoPath: String? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Request storage permission if needed
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
            != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, 
                arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE), 1001)
        }

        // Initialize UI components and set up listeners
        setupUI()
    }

    private fun setupUI() {
        // ... existing UI setup code ...

        btnStopEmulation.setOnClickListener {
            currentIsoPath?.let { isoPath ->
                val savePath = getSaveStatePath(isoPath)
                val saved = EmulatorBridge.saveState(savePath)
                if (saved) {
                    Toast.makeText(this, "Estado salvo automaticamente", Toast.LENGTH_SHORT).show()
                } else {
                    Toast.makeText(this, "Erro ao salvar estado", Toast.LENGTH_SHORT).show()
                }
            }
            val stopped = EmulatorBridge.stopEmulation()
            if (stopped) {
                Toast.makeText(this, "Emulação encerrada", Toast.LENGTH_SHORT).show()
            }
        }

        btnContinueGame.setOnClickListener {
            currentIsoPath?.let { isoPath ->
                val started = EmulatorBridge.startEmulation(isoPath)
                if (started) {
                    Toast.makeText(this, "Emulação iniciada", Toast.LENGTH_SHORT).show()
                    val savePath = getSaveStatePath(isoPath)
                    val restored = EmulatorBridge.loadState(savePath)
                    if (restored) {
                        Toast.makeText(this, "Jogo restaurado com sucesso", Toast.LENGTH_SHORT).show()
                    } else {
                        Toast.makeText(this, "Falha ao restaurar o estado", Toast.LENGTH_SHORT).show()
                    }
                } else {
                    Toast.makeText(this, "Erro ao iniciar emulação", Toast.LENGTH_SHORT).show()
                }
            }
        }
    }

    private fun checkForSaveState(isoPath: String) {
        val savePath = getSaveStatePath(isoPath)
        val saveFile = File(savePath)
        btnContinueGame.visibility = if (saveFile.exists()) View.VISIBLE else View.GONE
    }

    private fun getSaveStatePath(isoPath: String): String {
        val fileName = isoPath.substringAfterLast("/").substringBeforeLast(".")
        val saveDir = File(getExternalFilesDir(null), "saves")
        if (!saveDir.exists()) saveDir.mkdirs()
        return File(saveDir, "$fileName.state").absolutePath
    }

    // Call this method when an ISO is selected
    private fun onIsoSelected(isoPath: String) {
        currentIsoPath = isoPath
        checkForSaveState(isoPath)
    }

    // ... rest of the MainActivity implementation ...
} 