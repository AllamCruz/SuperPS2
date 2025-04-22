package com.superps2.util

import android.app.ActivityManager
import android.content.Context
import android.os.Build
import android.os.Environment
import android.os.StatFs
import com.superps2.SuperPS2App
import java.io.File

object DeviceInfo {
    private const val MIN_RAM_MB = 2048 // 2GB
    private const val MIN_STORAGE_MB = 1024 // 1GB
    private const val MIN_CPU_CORES = 4

    fun isDeviceCompatible(): Boolean {
        return hasEnoughRam() &&
               hasEnoughStorage() &&
               hasEnoughCores() &&
               isArmArchitecture()
    }

    private fun hasEnoughRam(): Boolean {
        val activityManager = SuperPS2App.instance.getSystemService(Context.ACTIVITY_SERVICE) as ActivityManager
        val memoryInfo = ActivityManager.MemoryInfo()
        activityManager.getMemoryInfo(memoryInfo)
        val totalRam = memoryInfo.totalMem / (1024 * 1024) // Convert to MB
        return totalRam >= MIN_RAM_MB
    }

    private fun hasEnoughStorage(): Boolean {
        val stat = StatFs(Environment.getDataDirectory().path)
        val availableBytes = stat.availableBytes
        val availableMB = availableBytes / (1024 * 1024)
        return availableMB >= MIN_STORAGE_MB
    }

    private fun hasEnoughCores(): Boolean {
        return Runtime.getRuntime().availableProcessors() >= MIN_CPU_CORES
    }

    private fun isArmArchitecture(): Boolean {
        val abis = Build.SUPPORTED_ABIS
        return abis.any { it.contains("arm") }
    }

    fun getDeviceInfo(): String {
        return """
            Dispositivo: ${Build.MANUFACTURER} ${Build.MODEL}
            Android: ${Build.VERSION.RELEASE} (API ${Build.VERSION.SDK_INT})
            Processador: ${Build.HARDWARE}
            Cores: ${Runtime.getRuntime().availableProcessors()}
            Arquitetura: ${Build.SUPPORTED_ABIS.joinToString()}
            RAM Total: ${getTotalRamMB()} MB
            Armazenamento Disponível: ${getAvailableStorageMB()} MB
        """.trimIndent()
    }

    private fun getTotalRamMB(): Long {
        val activityManager = SuperPS2App.instance.getSystemService(Context.ACTIVITY_SERVICE) as ActivityManager
        val memoryInfo = ActivityManager.MemoryInfo()
        activityManager.getMemoryInfo(memoryInfo)
        return memoryInfo.totalMem / (1024 * 1024)
    }

    private fun getAvailableStorageMB(): Long {
        val stat = StatFs(Environment.getDataDirectory().path)
        return stat.availableBytes / (1024 * 1024)
    }
} 