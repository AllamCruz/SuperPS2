package com.superps2.emu.model

data class GameFile(
    val name: String,
    val path: String,
    val sizeInMB: Long,
    val lastModified: Long
) 