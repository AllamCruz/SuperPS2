#ifndef SUPERPS2_VU1_H
#define SUPERPS2_VU1_H

#include <cstdint>

namespace ee {
    namespace vu1 {
        bool init();
        void reset();
        void shutdown();

        void uploadMicroprogram(const uint8_t* code, uint32_t size);
        void executeCycle();  // Simula uma instrução vetorial
        void run();           // Executa o microprograma
    }
}

#endif // SUPERPS2_VU1_H 