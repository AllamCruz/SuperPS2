package com.example.superps2.ui;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;
import android.widget.FrameLayout;

import com.example.superps2.R;
import com.example.superps2.PS2Bridge;

public class GameBootActivity extends Activity {
    private GameView gameView;
    private Thread emulationThread;
    private boolean isRunning = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        // Set fullscreen
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        
        setContentView(R.layout.activity_game_boot);
        
        // Initialize game view
        FrameLayout container = findViewById(R.id.gameContainer);
        gameView = new GameView(this);
        container.addView(gameView);
        
        // Start emulation
        String isoPath = getIntent().getStringExtra("iso_path");
        if (isoPath != null) {
            startEmulation(isoPath);
        }
    }

    private void startEmulation(final String isoPath) {
        isRunning = true;
        emulationThread = new Thread(new Runnable() {
            @Override
            public void run() {
                PS2Bridge.runEmulator(isoPath);
            }
        });
        emulationThread.start();
        gameView.startRendering();
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (isRunning) {
            gameView.stopRendering();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (isRunning) {
            gameView.startRendering();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        isRunning = false;
        if (emulationThread != null) {
            try {
                emulationThread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        gameView.stopRendering();
    }
} 