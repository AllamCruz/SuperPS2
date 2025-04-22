package com.superps2.emu.state

import android.content.Context
import android.net.Uri
import android.widget.Toast
import androidx.documentfile.provider.DocumentFile
import com.superps2.emu.core.StateBridge
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import java.io.File

class StateManager(private val context: Context) {
    
    companion object {
        private const val STATE_FILE_EXTENSION = "sps2"
    }

    suspend fun saveState(uri: Uri): Boolean = withContext(Dispatchers.IO) {
        try {
            val documentFile = DocumentFile.fromSingleUri(context, uri)
            requireNotNull(documentFile) { "Falha ao criar arquivo de documento" }
            
            val path = getRealPathFromUri(uri)
            requireNotNull(path) { "Falha ao obter caminho real do URI" }
            
            val success = StateBridge.saveState(path)
            
            withContext(Dispatchers.Main) {
                val message = if (success) "Estado salvo com sucesso" else "Falha ao salvar estado"
                Toast.makeText(context, message, Toast.LENGTH_SHORT).show()
            }
            
            success
        } catch (e: Exception) {
            withContext(Dispatchers.Main) {
                Toast.makeText(context, "Erro ao salvar estado: ${e.message}", Toast.LENGTH_SHORT).show()
            }
            false
        }
    }

    suspend fun loadState(uri: Uri): Boolean = withContext(Dispatchers.IO) {
        try {
            val path = getRealPathFromUri(uri)
            requireNotNull(path) { "Falha ao obter caminho real do URI" }
            
            val success = StateBridge.loadState(path)
            
            withContext(Dispatchers.Main) {
                val message = if (success) "Estado carregado com sucesso" else "Falha ao carregar estado"
                Toast.makeText(context, message, Toast.LENGTH_SHORT).show()
            }
            
            success
        } catch (e: Exception) {
            withContext(Dispatchers.Main) {
                Toast.makeText(context, "Erro ao carregar estado: ${e.message}", Toast.LENGTH_SHORT).show()
            }
            false
        }
    }

    private fun getRealPathFromUri(uri: Uri): String? {
        // Implementação depende da versão do Android e do tipo de URI
        // Pode ser necessário lidar com diferentes esquemas de URI (content://, file://, etc.)
        return context.contentResolver.openFileDescriptor(uri, "r")?.use { 
            File(uri.path ?: return null).absolutePath
        }
    }
} 