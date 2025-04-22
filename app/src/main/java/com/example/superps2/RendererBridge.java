package com.example.superps2;

import android.view.Surface;

public class RendererBridge {
    static {
        System.loadLibrary("superps2");
    }

    public static native boolean initRenderer(Surface surface, int width, int height);
    public static native void renderFrame(byte[] framebuffer, int width, int height);
    public static native void shutdownRenderer();
} 