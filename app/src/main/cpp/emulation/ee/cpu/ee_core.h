#ifndef SUPERPS2_EE_CORE_H
#define SUPERPS2_EE_CORE_H

#include <cstdint>

namespace ee {
    namespace cpu {
        bool init();
        void shutdown();
        void reset();

        void step();  // Executa uma única instrução
        void run();   // Loop principal (modo interpretado)
    }
}

#endif // SUPERPS2_EE_CORE_H 