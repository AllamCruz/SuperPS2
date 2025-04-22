package com.superps2.emu.loader

import android.content.Context
import android.net.Uri
import android.provider.DocumentsContract
import android.util.Log
import com.superps2.emu.model.GameFile
import java.io.File

object IsoLoader {

    private val allowedExtensions = listOf("iso", "bin", "img")

    fun loadIsoFilesFromFolder(context: Context, folderUri: Uri): List<GameFile> {
        val result = mutableListOf<GameFile>()

        val childrenUri = DocumentsContract.buildChildDocumentsUriUsingTree(
            folderUri,
            DocumentsContract.getTreeDocumentId(folderUri)
        )

        val cursor = context.contentResolver.query(
            childrenUri,
            arrayOf(
                DocumentsContract.Document.COLUMN_DISPLAY_NAME,
                DocumentsContract.Document.COLUMN_DOCUMENT_ID,
                DocumentsContract.Document.COLUMN_SIZE,
                DocumentsContract.Document.COLUMN_LAST_MODIFIED
            ),
            null,
            null,
            null
        )

        cursor?.use {
            val nameIndex = it.getColumnIndex(DocumentsContract.Document.COLUMN_DISPLAY_NAME)
            val docIdIndex = it.getColumnIndex(DocumentsContract.Document.COLUMN_DOCUMENT_ID)
            val sizeIndex = it.getColumnIndex(DocumentsContract.Document.COLUMN_SIZE)
            val modifiedIndex = it.getColumnIndex(DocumentsContract.Document.COLUMN_LAST_MODIFIED)

            while (it.moveToNext()) {
                val name = it.getString(nameIndex) ?: continue
                val docId = it.getString(docIdIndex)
                val size = it.getLong(sizeIndex)
                val modified = it.getLong(modifiedIndex)

                val extension = name.substringAfterLast('.', "").lowercase()
                if (allowedExtensions.contains(extension)) {
                    val documentUri = DocumentsContract.buildDocumentUriUsingTree(folderUri, docId)
                    val realPath = documentUri.toString()

                    result.add(
                        GameFile(
                            name = name,
                            path = realPath,
                            sizeInMB = size / (1024 * 1024),
                            lastModified = modified
                        )
                    )
                }
            }
        } ?: run {
            Log.e("IsoLoader", "Failed to query folder: $folderUri")
        }

        return result
    }
} 