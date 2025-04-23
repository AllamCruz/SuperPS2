#pragma once

#include <string>
#include <stdint.h>

namespace ps2core {
    bool init();
    bool loadISO(const std::string& isoPath);
    void run();
    void shutdown();
    
    // Controla o estado da emulação
    void pauseEmulation();
    void resumeEmulation();
    void stopEmulation();
}