#ifndef SUPERPS2_PS2CORE_H
#define SUPERPS2_PS2CORE_H

#include <string>

namespace ps2core {
    bool init(const std::string& isoPath);
    void startLoop(); // Roda o ciclo principal da emulação
    void stop();
    bool isRunning();
}

#endif //SUPERPS2_PS2CORE_H