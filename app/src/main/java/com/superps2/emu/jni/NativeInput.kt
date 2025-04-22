package com.superps2.emu.jni

object NativeInput {
    init { System.loadLibrary("superps2") }
    external fun onAxisEvent(axis: Int, value: Float)
} 