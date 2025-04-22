package com.superps2.emu.input

import android.content.Context
import android.view.KeyEvent
import android.view.MotionEvent
import android.view.View
import android.view.ViewConfiguration
import android.widget.ImageButton
import android.widget.SeekBar
import com.superps2.emu.core.NativeInput
import com.superps2.emu.R

/**
 * Gerencia entrada de controles touch e físicos.
 */
class InputManager(private val context: Context) {
    private val touchSlop = ViewConfiguration.get(context).scaledTouchSlop

    fun setupListeners(overlay: View) {
        // D-Pad
        val btnUp = overlay.findViewById<ImageButton>(R.id.btnUp)
        val btnDown = overlay.findViewById<ImageButton>(R.id.btnDown)
        val btnLeft = overlay.findViewById<ImageButton>(R.id.btnLeft)
        val btnRight = overlay.findViewById<ImageButton>(R.id.btnRight)

        listOf(btnUp to PS2Button.UP, btnDown to PS2Button.DOWN,
               btnLeft to PS2Button.LEFT, btnRight to PS2Button.RIGHT).forEach { (btn, code) ->
            btn.setOnTouchListener { _, ev ->
                when (ev.action) {
                    MotionEvent.ACTION_DOWN -> NativeInput.onButtonEvent(code, true)
                    MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> NativeInput.onButtonEvent(code, false)
                }
                true
            }
        }

        // PS2 Buttons
        val btnTriangle = overlay.findViewById<ImageButton>(R.id.btnTriangle)
        val btnCircle = overlay.findViewById<ImageButton>(R.id.btnCircle)
        val btnCross = overlay.findViewById<ImageButton>(R.id.btnCross)
        val btnSquare = overlay.findViewById<ImageButton>(R.id.btnSquare)

        listOf(btnTriangle to PS2Button.TRIANGLE, btnCircle to PS2Button.CIRCLE,
               btnCross to PS2Button.CROSS, btnSquare to PS2Button.SQUARE).forEach { (btn, code) ->
            btn.setOnTouchListener { _, ev ->
                when (ev.action) {
                    MotionEvent.ACTION_DOWN -> NativeInput.onButtonEvent(code, true)
                    MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> NativeInput.onButtonEvent(code, false)
                }
                true
            }
        }

        // Analog sticks
        val stickLeft = overlay.findViewById<SeekBar>(R.id.stickLeft)
        val stickRight = overlay.findViewById<SeekBar>(R.id.stickRight)

        stickLeft.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(sb: SeekBar, progress: Int, fromUser: Boolean) {
                val value = (progress - 50) / 50f // -1..1
                NativeInput.onAxisEvent(PS2Axis.LX, value)
            }
            override fun onStartTrackingTouch(sb: SeekBar) {}
            override fun onStopTrackingTouch(sb: SeekBar) {}
        })

        stickRight.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(sb: SeekBar, progress: Int, fromUser: Boolean) {
                val value = (progress - 50) / 50f // -1..1
                NativeInput.onAxisEvent(PS2Axis.RX, value)
            }
            override fun onStartTrackingTouch(sb: SeekBar) {}
            override fun onStopTrackingTouch(sb: SeekBar) {}
        })
    }

    fun onGenericMotionEvent(event: MotionEvent): Boolean {
        // Exemplo: eixos de gamepad
        val lx = event.getAxisValue(MotionEvent.AXIS_X)
        val ly = event.getAxisValue(MotionEvent.AXIS_Y)
        NativeInput.onAxisEvent(PS2Axis.LX, lx)
        NativeInput.onAxisEvent(PS2Axis.LY, ly)
        return true
    }

    fun onKeyEvent(keyCode: Int, isPressed: Boolean): Boolean {
        val ps2Code = when (keyCode) {
            KeyEvent.KEYCODE_BUTTON_A -> PS2Button.CROSS
            KeyEvent.KEYCODE_BUTTON_B -> PS2Button.CIRCLE
            KeyEvent.KEYCODE_BUTTON_X -> PS2Button.SQUARE
            KeyEvent.KEYCODE_BUTTON_Y -> PS2Button.TRIANGLE
            KeyEvent.KEYCODE_DPAD_UP -> PS2Button.UP
            KeyEvent.KEYCODE_DPAD_DOWN -> PS2Button.DOWN
            KeyEvent.KEYCODE_DPAD_LEFT -> PS2Button.LEFT
            KeyEvent.KEYCODE_DPAD_RIGHT -> PS2Button.RIGHT
            else -> return false
        }
        NativeInput.onButtonEvent(ps2Code, isPressed)
        return true
    }
}

// PS2 button codes
object PS2Button {
    const val UP = 0
    const val DOWN = 1
    const val LEFT = 2
    const val RIGHT = 3
    const val CROSS = 4
    const val CIRCLE = 5
    const val TRIANGLE = 6
    const val SQUARE = 7
}

// PS2 axis codes
object PS2Axis {
    const val LX = 0
    const val LY = 1
    const val RX = 2
    const val RY = 3
}
