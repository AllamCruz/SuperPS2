package com.superps2.emu.core

object StateBridge {
    init {
        System.loadLibrary("superps2")
    }

    external fun saveState(path: String): Boolean
    external fun loadState(path: String): Boolean
} 