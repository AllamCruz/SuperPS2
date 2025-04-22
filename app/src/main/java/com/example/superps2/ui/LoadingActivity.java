package com.example.superps2.ui;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.example.superps2.R;
import com.example.superps2.PS2Bridge;

public class LoadingActivity extends Activity {
    private ProgressBar progressBar;
    private TextView statusText;
    private Handler handler;
    private String isoPath;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_loading);

        progressBar = findViewById(R.id.progressBar);
        statusText = findViewById(R.id.tvStatus);
        handler = new Handler(Looper.getMainLooper());

        // Get ISO path from intent
        isoPath = getIntent().getStringExtra("iso_path");
        if (isoPath == null) {
            finish();
            return;
        }

        // Start loading process
        new Thread(this::loadISO).start();
    }

    private void loadISO() {
        updateStatus("Loading ISO...");
        updateProgress(10);

        // Initialize emulator
        if (!PS2Bridge.init()) {
            updateStatus("Failed to initialize emulator");
            updateProgress(100);
            return;
        }
        updateProgress(30);

        // Load ISO
        if (!PS2Bridge.loadISO(isoPath)) {
            updateStatus("Failed to load ISO");
            updateProgress(100);
            return;
        }
        updateProgress(60);

        // Start emulator
        updateStatus("Starting emulator...");
        if (!PS2Bridge.runEmulator()) {
            updateStatus("Failed to start emulator");
            updateProgress(100);
            return;
        }
        updateProgress(90);

        // Success - launch game activity
        updateStatus("Starting game...");
        updateProgress(100);
        
        handler.postDelayed(() -> {
            Intent intent = new Intent(LoadingActivity.this, GameBootActivity.class);
            startActivity(intent);
            finish();
        }, 1000);
    }

    private void updateStatus(final String status) {
        handler.post(() -> statusText.setText(status));
    }

    private void updateProgress(final int progress) {
        handler.post(() -> progressBar.setProgress(progress));
    }
} 