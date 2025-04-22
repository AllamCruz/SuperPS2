package com.superps2.emu.graphics

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class GLRenderer : GLSurfaceView.Renderer {

    external fun nativeInit()
    external fun nativeRender()
    external fun nativeShutdown()

    override fun onSurfaceCreated(unused: GL10?, config: EGLConfig?) {
        nativeInit()
    }

    override fun onDrawFrame(unused: GL10?) {
        nativeRender()
    }

    override fun onSurfaceChanged(unused: GL10?, width: Int, height: Int) {
        // Resize logic can be handled here in future
    }

    protected fun finalize() {
        nativeShutdown()
    }

    companion object {
        init {
            System.loadLibrary("superps2")
        }
    }
} 