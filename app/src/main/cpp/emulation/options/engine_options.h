#ifndef SUPERPS2_ENGINE_OPTIONS_H
#define SUPERPS2_ENGINE_OPTIONS_H

namespace options {
    // Escala interna (1.0 =  native, 2.0 = 2×, etc.)
    void setResolutionScale(float scale);

    // Limita FPS (0 = uncapped, >0 = cap)
    void setFrameLimit(int fps);

    // Liga/desliga VSync
    void setVSyncEnabled(bool enabled);

    // Habilita otimizações de multi‑thread
    void setMultiThreadEnabled(bool enabled);

    // Habilita instruções NEON
    void setNeonOptimizations(bool enabled);

    // Função de init/shutdown (se necessário)
    void init();
    void shutdown();
}

#endif // SUPERPS2_ENGINE_OPTIONS_H 