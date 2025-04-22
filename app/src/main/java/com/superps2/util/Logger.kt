package com.superps2.util

import android.util.Log
import com.superps2.BuildConfig
import java.io.File
import java.text.SimpleDateFormat
import java.util.*

object Logger {
    private const val TAG_PREFIX = "SuperPS2"
    private val logFile: File by lazy {
        File(SuperPS2App.instance.getExternalFilesDir(null), "logs/superps2.log").apply {
            parentFile?.mkdirs()
        }
    }
    private val dateFormat = SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale.getDefault())

    fun d(tag: String, message: String) {
        if (BuildConfig.DEBUG) {
            Log.d("$TAG_PREFIX-$tag", message)
            writeToFile("DEBUG", tag, message)
        }
    }

    fun i(tag: String, message: String) {
        Log.i("$TAG_PREFIX-$tag", message)
        writeToFile("INFO", tag, message)
    }

    fun w(tag: String, message: String) {
        Log.w("$TAG_PREFIX-$tag", message)
        writeToFile("WARN", tag, message)
    }

    fun e(tag: String, message: String, error: Throwable? = null) {
        Log.e("$TAG_PREFIX-$tag", message, error)
        writeToFile("ERROR", tag, "$message\n${error?.stackTraceToString()}")
    }

    private fun writeToFile(level: String, tag: String, message: String) {
        try {
            val timestamp = dateFormat.format(Date())
            val logMessage = "[$timestamp] [$level] [$tag] $message\n"
            logFile.appendText(logMessage)
        } catch (e: Exception) {
            Log.e("$TAG_PREFIX-Logger", "Erro ao escrever no arquivo de log", e)
        }
    }

    fun clearLogs() {
        try {
            if (logFile.exists()) {
                logFile.delete()
                logFile.createNewFile()
            }
        } catch (e: Exception) {
            Log.e("$TAG_PREFIX-Logger", "Erro ao limpar logs", e)
        }
    }
} 