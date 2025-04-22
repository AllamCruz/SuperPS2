package com.example.superps2.ui;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.SurfaceHolder;

public class GameView extends GLSurfaceView {
    private boolean isRendering = false;

    public GameView(Context context) {
        super(context);
        init();
    }

    public GameView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init() {
        setEGLContextClientVersion(2);
        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        getHolder().setFormat(PixelFormat.RGBA_8888);
        setZOrderOnTop(true);
    }

    public void startRendering() {
        if (!isRendering) {
            isRendering = true;
            setRenderMode(RENDERMODE_WHEN_DIRTY);
        }
    }

    public void stopRendering() {
        if (isRendering) {
            isRendering = false;
            setRenderMode(RENDERMODE_WHEN_DIRTY);
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        stopRendering();
        super.surfaceDestroyed(holder);
    }
} 