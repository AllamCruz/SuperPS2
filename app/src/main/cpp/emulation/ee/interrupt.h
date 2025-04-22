#ifndef SUPERPS2_INTERRUPT_H
#define SUPERPS2_INTERRUPT_H

#include <cstdint>

namespace ee {
    namespace interrupt {
        // Fontes de interrupção do EE
        enum Source {
            INT_VBLANK_START = 0,
            INT_VBLANK_END = 1,
            INT_TIMER0 = 2,
            INT_TIMER1 = 3,
            INT_DMA0 = 4,
            INT_DMA1 = 5,
            INT_DMA2 = 6,
            INT_DMA3 = 7,
            INT_SBUS = 8,
            INT_OTHER = 31
        };

        // Inicialização e liberação
        bool init();
        void shutdown();

        // Define se o sistema está aguardando uma interrupção
        void setInterruptWaiting(bool waiting);

        // Solicita uma interrupção de um tipo específico
        void request(uint32_t source);

        // Executa as interrupções pendentes
        void process();

        // Limpa uma interrupção específica
        void acknowledge(uint32_t source);

        // Verifica se há interrupção pendente
        bool hasPending();

        // Reset
        void reset();
    }
}

#endif // SUPERPS2_INTERRUPT_H 