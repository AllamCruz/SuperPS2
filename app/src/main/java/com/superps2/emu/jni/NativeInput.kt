package com.superps2.emu.jni

object NativeInput {
    init { System.loadLibrary("native-lib") }
    external fun onAxisEvent(axis: Int, value: Float)
} 