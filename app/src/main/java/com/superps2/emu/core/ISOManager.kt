package com.superps2.emu.core

import android.content.Context
import android.content.SharedPreferences
import android.net.Uri
import android.provider.DocumentsContract
import android.util.Log
import androidx.documentfile.provider.DocumentFile
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import java.text.DecimalFormat

// Representa um jogo ISO encontrado
data class ISOGame(
    val name: String,
    val uri: String,
    val sizeInMB: Float,
    val lastModified: Long
)

object ISOManager {

    private const val TAG = "ISOManager"
    private const val PREFS_NAME = "SuperPS2Settings"
    private const val ISO_FOLDER_KEY = "iso_folder_uri"
    private const val ISO_CACHE_KEY = "iso_cache_json"

    /**
     * Escaneia arquivos ISO válidos (.iso, .bin, .img) no diretório salvo
     * Retorna uma lista de jogos encontrados, ou vazia se nada for encontrado.
     */
    suspend fun scanISOFiles(context: Context): List<ISOGame> = withContext(Dispatchers.IO) {
        val prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE)
        val folderUriString = prefs.getString(ISO_FOLDER_KEY, null) ?: return@withContext emptyList()

        try {
            val folderUri = Uri.parse(folderUriString)
            context.contentResolver.takePersistableUriPermission(
                folderUri,
                Intent.FLAG_GRANT_READ_URI_PERMISSION or Intent.FLAG_GRANT_WRITE_URI_PERMISSION
            )

            val rootDir = DocumentFile.fromTreeUri(context, folderUri) ?: return@withContext emptyList()

            val isoList = mutableListOf<ISOGame>()
            walkDirectory(rootDir, isoList)

            // Ordena por nome
            val sortedList = isoList.sortedBy { it.name.lowercase() }

            // Salva em cache
            salvarCache(sortedList, context)

            Log.d(TAG, "Foram encontrados ${sortedList.size} arquivos ISO.")
            return@withContext sortedList

        } catch (e: SecurityException) {
            Log.e(TAG, "Permissão revogada para a pasta ISO.", e)
            return@withContext emptyList()
        } catch (e: Exception) {
            Log.e(TAG, "Erro ao escanear arquivos ISO.", e)
            return@withContext emptyList()
        }
    }

    /**
     * Varredura recursiva de diretórios usando DocumentFile (SAF).
     * Adiciona todos os arquivos ISO válidos à lista.
     */
    private fun walkDirectory(dir: DocumentFile, list: MutableList<ISOGame>) {
        if (!dir.isDirectory) return

        for (file in dir.listFiles()) {
            if (file.isDirectory) {
                walkDirectory(file, list)
            } else if (file.name?.lowercase()?.endsWith(".iso") == true ||
                file.name?.lowercase()?.endsWith(".bin") == true ||
                file.name?.lowercase()?.endsWith(".img") == true
            ) {
                val iso = getGameMetadata(file)
                list.add(iso)
                Log.d(TAG, "Jogo encontrado: ${iso.name}")
            }
        }
    }

    /**
     * Extrai os metadados do arquivo ISO usando DocumentFile
     */
    private fun getGameMetadata(file: DocumentFile): ISOGame {
        val name = file.name ?: "Desconhecido"
        val uri = file.uri.toString()
        val sizeInMB = file.length().toFloat() / (1024 * 1024)
        val lastModified = file.lastModified()
        val formattedSize = DecimalFormat("#.##").format(sizeInMB).replace(",", ".").toFloat()

        return ISOGame(name, uri, formattedSize, lastModified)
    }

    /**
     * Salva a lista de jogos em JSON no SharedPreferences
     */
    fun salvarCache(lista: List<ISOGame>, context: Context) {
        val prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE)
        val editor = prefs.edit()
        val json = Gson().toJson(lista)
        editor.putString(ISO_CACHE_KEY, json)
        editor.apply()
        Log.d(TAG, "Cache de jogos ISO salvo com sucesso.")
    }

    /**
     * Recupera a lista de jogos do cache salvo, se existir
     */
    fun carregarCache(context: Context): List<ISOGame> {
        val prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE)
        val json = prefs.getString(ISO_CACHE_KEY, null) ?: return emptyList()
        return try {
            val type = object : TypeToken<List<ISOGame>>() {}.type
            Gson().fromJson(json, type)
        } catch (e: Exception) {
            Log.e(TAG, "Erro ao carregar cache de jogos.", e)
            emptyList()
        }
    }
}
