package com.superps2.emu.ui

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.google.android.material.floatingactionbutton.FloatingActionButton
import com.superps2.emu.R
import com.superps2.emu.graphics.GLGameView
import com.superps2.emu.loader.IsoLoader
import com.superps2.emu.bridge.EmulatorBridge

class MainActivity : AppCompatActivity() {

    private lateinit var glGameView: GLGameView
    private lateinit var btnStopEmulation: Button
    private val REQUEST_CODE_OPEN_DIRECTORY = 1001

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        glGameView = GLGameView(this)
        btnStopEmulation = findViewById(R.id.btnStopEmulation)

        val addButton: FloatingActionButton = findViewById(R.id.btnAddIso)
        addButton.setOnClickListener {
            val intent = Intent(Intent.ACTION_OPEN_DOCUMENT_TREE)
            intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION)
            intent.addFlags(Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION)
            intent.addFlags(Intent.FLAG_GRANT_PREFIX_URI_PERMISSION)
            startActivityForResult(intent, REQUEST_CODE_OPEN_DIRECTORY)
        }

        btnStopEmulation.setOnClickListener {
            val stopped = EmulatorBridge.stopEmulation()
            if (stopped) {
                Toast.makeText(this, "Emulação encerrada", Toast.LENGTH_SHORT).show()
                btnStopEmulation.visibility = android.view.View.GONE
            }
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        if (requestCode == REQUEST_CODE_OPEN_DIRECTORY && resultCode == Activity.RESULT_OK) {
            data?.data?.let { uri ->
                contentResolver.takePersistableUriPermission(
                    uri,
                    Intent.FLAG_GRANT_READ_URI_PERMISSION
                )

                // Load games from selected folder
                val games = IsoLoader.loadIsoFilesFromFolder(this, uri)
                Log.i("MainActivity", "ISOs encontrados: ${games.size}")

                gameAdapter = GameAdapter(games) { selectedGame ->
                    try {
                        val started = EmulatorBridge.startEmulation(selectedGame.path)
                        if (started) {
                            Toast.makeText(this, "Jogo iniciado: ${selectedGame.name}", Toast.LENGTH_SHORT).show()
                            btnStopEmulation.visibility = android.view.View.VISIBLE
                            // TODO: Iniciar GameBootActivity aqui
                        } else {
                            Toast.makeText(this, "Erro ao iniciar: ${selectedGame.name}", Toast.LENGTH_SHORT).show()
                        }
                    } catch (e: Exception) {
                        Toast.makeText(this, "Erro: ${e.message}", Toast.LENGTH_SHORT).show()
                    }
                }
            }
        }
    }

    override fun onPause() {
        super.onPause()
        glGameView.onPauseView()
    }

    override fun onResume() {
        super.onResume()
        glGameView.onResumeView()
    }
} 