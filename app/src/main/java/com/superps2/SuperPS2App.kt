package com.superps2

import android.app.ActivityManager
import android.app.Application
import android.content.Context
import android.os.Build
import com.superps2.util.Logger
import com.superps2.util.DeviceInfo

class SuperPS2App : Application() {
    companion object {
        lateinit var instance: SuperPS2App
            private set
    }

    override fun onCreate() {
        super.onCreate()
        instance = this
        initializeApp()
    }

    private fun initializeApp() {
        Logger.d("App", "Inicializando SuperPS2 v0.1.0-beta")
        
        // Verificar compatibilidade do dispositivo
        if (!DeviceInfo.isDeviceCompatible()) {
            Logger.e("App", "Dispositivo não compatível")
            return
        }

        // Inicializar componentes
        initializeComponents()
    }

    private fun initializeComponents() {
        // TODO: Inicializar outros componentes necessários
    }

    override fun onTrimMemory(level: Int) {
        super.onTrimMemory(level)
        when (level) {
            TRIM_MEMORY_RUNNING_CRITICAL,
            TRIM_MEMORY_COMPLETE -> {
                Logger.d("Memory", "Memória crítica, liberando recursos")
                System.gc()
            }
        }
    }

    override fun onLowMemory() {
        super.onLowMemory()
        Logger.w("Memory", "Memória baixa detectada")
        System.gc()
    }
} 