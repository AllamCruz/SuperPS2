package com.superps2.emu.ui.views

import android.content.Context
import android.os.VibrationEffect
import android.os.Vibrator
import android.util.AttributeSet
import android.view.*
import android.widget.ImageButton
import android.widget.FrameLayout
import androidx.core.content.ContextCompat
import com.superps2.emu.R
import com.superps2.emu.core.InputBridge
import kotlin.math.abs
import androidx.constraintlayout.widget.ConstraintLayout
import com.superps2.emu.jni.PS2Bridge
import android.content.Context.VIBRATOR_SERVICE

class EmulationOverlayView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null,
    defStyleAttr: Int = 0
) : ConstraintLayout(context, attrs, defStyleAttr) {

    private var touchSlop = ViewConfiguration.get(context).scaledTouchSlop
    private var vibrator = ContextCompat.getSystemService(context, android.os.Vibrator::class.java)
    private var lastTouchX = 0f
    private var lastTouchY = 0f
    private var isDragging = false
    private val pressedButtons = mutableSetOf<Int>()
    private val buttons = mutableMapOf<Int, ImageButton>()

    init {
        inflate(context, R.layout.overlay_emulation, this)
        setupButtons()
    }

    private fun setupButtons() {
        // Map button IDs to PS2 button codes
        val buttonMap = mapOf(
            R.id.btnX to PS2Bridge.BUTTON_X,
            R.id.btnCircle to PS2Bridge.BUTTON_CIRCLE,
            R.id.btnSquare to PS2Bridge.BUTTON_SQUARE,
            R.id.btnTriangle to PS2Bridge.BUTTON_TRIANGLE,
            R.id.btnL1 to PS2Bridge.BUTTON_L1,
            R.id.btnR1 to PS2Bridge.BUTTON_R1,
            R.id.btnL2 to PS2Bridge.BUTTON_L2,
            R.id.btnR2 to PS2Bridge.BUTTON_R2,
            R.id.btnStart to PS2Bridge.BUTTON_START,
            R.id.btnSelect to PS2Bridge.BUTTON_SELECT
        )

        // Setup touch listeners for each button
        buttonMap.forEach { (viewId, buttonCode) ->
            findViewById<ImageButton>(viewId)?.let { button ->
                buttons[buttonCode] = button
                button.setOnTouchListener { _, event ->
                    when (event.action) {
                        MotionEvent.ACTION_DOWN -> {
                            vibrate()
                            PS2Bridge.sendButtonEvent(buttonCode, true)
                            true
                        }
                        MotionEvent.ACTION_UP -> {
                            PS2Bridge.sendButtonEvent(buttonCode, false)
                            true
                        }
                        else -> false
                    }
                }
            }
        }
    }

    private fun vibrate() {
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
            vibrator?.vibrate(VibrationEffect.createOneShot(50, VibrationEffect.DEFAULT_AMPLITUDE))
        } else {
            @Suppress("DEPRECATION")
            vibrator?.vibrate(50)
        }
    }

    override fun onInterceptTouchEvent(ev: MotionEvent): Boolean {
        val action = ev.action
        if (action == MotionEvent.ACTION_DOWN) {
            lastTouchX = ev.x
            lastTouchY = ev.y
            isDragging = false
            return false
        }

        if (action == MotionEvent.ACTION_MOVE) {
            val dx = abs(ev.x - lastTouchX)
            val dy = abs(ev.y - lastTouchY)
            if (dx > touchSlop || dy > touchSlop) {
                isDragging = true
                return true
            }
        }

        return false
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        if (isDragging) {
            when (event.action) {
                MotionEvent.ACTION_MOVE -> {
                    // Handle dragging if needed
                    return true
                }
                MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                    isDragging = false
                    return true
                }
            }
        }
        return super.onTouchEvent(event)
    }

    override fun onDetachedFromWindow() {
        super.onDetachedFromWindow()
        // Release all pressed buttons when view is detached
        pressedButtons.forEach { buttonCode ->
            InputBridge.setButtonState(buttonCode, false)
        }
        pressedButtons.clear()
    }
} 