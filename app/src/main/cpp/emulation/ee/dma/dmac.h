#ifndef SUPERPS2_DMAC_H
#define SUPERPS2_DMAC_H

#include <cstdint>

namespace ee {
    namespace dma {

        enum class Channel : uint8_t {
            VIF0 = 0,
            VIF1,
            GIF,
            FROMIPU,
            TOIPU,
            SIF0,
            SIF1,
            SIF2,
            SPR,
            COUNT
        };

        bool init();
        void reset();
        void shutdown();

        void startTransfer(Channel channel, uint32_t srcAddr, uint32_t dstAddr, uint32_t size);
        void update(); // Chamada periódica para manter estado
        void simulateCycle(); // Simula 1 ciclo DMA
        bool isTransferComplete(Channel channel);
    }
}

#endif // SUPERPS2_DMAC_H 