package com.superps2.emu.ui.views

import android.content.Context
import android.graphics.*
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View
import com.superps2.emu.jni.NativeInput
import kotlin.math.*

class AnalogStickView @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0
) : View(context, attrs, defStyleAttr) {

    // Center and radius of the active area
    private var centerX = 0f
    private var centerY = 0f
    private var stickRadius = 0f

    // Current position of the "knob"
    private var knobX = 0f
    private var knobY = 0f

    private val knobPaint = Paint().apply {
        style = Paint.Style.FILL
        color = Color.parseColor("#88FFFFFF")
    }

    override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
        centerX = w / 2f
        centerY = h / 2f
        stickRadius = min(w, h) / 2f
        resetKnob()
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        // Draw the knob
        canvas.drawCircle(knobX, knobY, stickRadius / 3f, knobPaint)
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        when (event.actionMasked) {
            MotionEvent.ACTION_DOWN, MotionEvent.ACTION_MOVE -> {
                val dx = event.x - centerX
                val dy = event.y - centerY
                val dist = sqrt(dx*dx + dy*dy)
                val max = stickRadius
                val ratio = if (dist > max) max / dist else 1f
                knobX = centerX + dx * ratio
                knobY = centerY + dy * ratio
                invalidate()

                // Normalize to [-1,1]
                val normX = (knobX - centerX) / max
                val normY = (knobY - centerY) / max
                NativeInput.onAxisEvent(AxisCode.AXIS_X, normX)
                NativeInput.onAxisEvent(AxisCode.AXIS_Y, -normY) // inverted vertical
            }
            MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                resetKnob()
                invalidate()
                NativeInput.onAxisEvent(AxisCode.AXIS_X, 0f)
                NativeInput.onAxisEvent(AxisCode.AXIS_Y, 0f)
            }
        }
        return true
    }

    private fun resetKnob() {
        knobX = centerX
        knobY = centerY
    }

    object AxisCode {
        const val AXIS_X = 0
        const val AXIS_Y = 1
    }
} 