package com.superps2.emu

import androidx.appcompat.app.AppCompatActivity
import com.superps2.emu.graphics.GLGameView

class MainActivity : AppCompatActivity() {

    companion object {
        init {
            System.loadLibrary("superps2")
        }
    }

    private lateinit var glGameView: GLGameView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        glGameView = GLGameView(this)
        setContentView(glGameView)
    }

    override fun onPause() {
        super.onPause()
        glGameView.onPauseView()
    }

    override fun onResume() {
        super.onResume()
        glGameView.onResumeView()
    }
}
