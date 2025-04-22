package com.superps2.emu.memorycard

import android.content.Context
import com.superps2.emu.core.MemoryCardBridge

object MemoryCardManager {
    fun initialize(context: Context) {
        val base = context.filesDir.absolutePath + "/memorycards"
        MemoryCardBridge.initCardSystem(base)
    }

    fun open(slot: Int) = MemoryCardBridge.openCard(slot)
    fun getSaves() = MemoryCardBridge.listSaves().toList()
    fun loadSave(name: String, block: Int) = MemoryCardBridge.readSave(name, block)
    fun saveBlock(name: String, block: Int, data: ByteArray) = MemoryCardBridge.writeSave(name, block, data)
} 