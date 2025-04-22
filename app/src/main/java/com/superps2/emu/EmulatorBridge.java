package com.superps2.emu;

public class EmulatorBridge {
    // ... existing methods ...

    public native boolean saveState(String savePath);
    public native boolean loadState(String savePath);

    static {
        System.loadLibrary("superps2");
    }
} 