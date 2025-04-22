#ifndef SUPERPS2_VU0_H
#define SUPERPS2_VU0_H

#include <cstdint>

namespace ee {
    namespace vu0 {
        bool init();
        void reset();
        void shutdown();

        void executeCycle(); // Executa uma instrução simulada
        void run();          // Loop de execução para simulação paralela

        void setRegister(uint8_t reg, float value);
        float getRegister(uint8_t reg);
    }
}

#endif // SUPERPS2_VU0_H 