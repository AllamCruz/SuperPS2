#pragma once

#include <stdint.h>

namespace ee {
    namespace cpu {
        bool init();
        void shutdown();
        void reset();

        void step();  // Executa uma única instrução
        void run();   // Loop principal (modo interpretado)
    }
} 