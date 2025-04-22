package com.superps2.emu.graphics

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet

class GLGameView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null
) : GLSurfaceView(context, attrs) {

    private val renderer: GLRenderer

    init {
        setEGLContextClientVersion(3)
        renderer = GLRenderer()
        setRenderer(renderer)
        renderMode = RENDERMODE_CONTINUOUSLY
    }

    fun onPauseView() {
        super.onPause()
    }

    fun onResumeView() {
        super.onResume()
    }
} 