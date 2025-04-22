package com.superps2.emu.core

object MemoryCardBridge {
    init {
        System.loadLibrary("superps2")
    }

    external fun initCardSystem(basePath: String): Boolean
    external fun openCard(slot: Int): Boolean
    external fun listSaves(): Array<String>
    external fun readSave(name: String, blockIndex: Int): ByteArray
    external fun writeSave(name: String, blockIndex: Int, data: ByteArray): Boolean
} 