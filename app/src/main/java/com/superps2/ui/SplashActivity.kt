package com.superps2.ui

import android.content.Intent
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import com.superps2.R
import com.superps2.util.DeviceInfo
import com.superps2.util.Logger

class SplashActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_splash)

        Handler(Looper.getMainLooper()).postDelayed({
            checkDeviceCompatibility()
        }, 1500) // 1.5 segundos de splash screen
    }

    private fun checkDeviceCompatibility() {
        if (!DeviceInfo.isDeviceCompatible()) {
            Logger.e("Splash", "Dispositivo não compatível")
            showIncompatibleDeviceDialog()
        } else {
            Logger.i("Splash", "Dispositivo compatível: ${DeviceInfo.getDeviceInfo()}")
            startMainActivity()
        }
    }

    private fun showIncompatibleDeviceDialog() {
        AlertDialog.Builder(this)
            .setTitle("Dispositivo Não Compatível")
            .setMessage("""
                Seu dispositivo não atende aos requisitos mínimos para executar o SuperPS2.
                
                Requisitos mínimos:
                - 2GB de RAM
                - 1GB de armazenamento livre
                - Processador ARM com 4 ou mais núcleos
                
                Informações do seu dispositivo:
                ${DeviceInfo.getDeviceInfo()}
            """.trimIndent())
            .setPositiveButton("OK") { _, _ ->
                finish()
            }
            .setCancelable(false)
            .show()
    }

    private fun startMainActivity() {
        startActivity(Intent(this, MainActivity::class.java))
        finish()
    }
} 