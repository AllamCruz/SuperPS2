#ifndef SUPERPS2_DMA_H
#define SUPERPS2_DMA_H

#include <cstdint>

namespace dma {
  enum Channel {
    VIF0    = 0,
    VIF1    = 1,
    GIF0    = 2,
    GPU     = 3,
    CDVD    = 4,
    SPU2    = 5,
    // … demais canais (SoundIN, OTC, etc.)
  };

  bool init();
  bool transfer(Channel channel, uint32_t srcAddr, uint32_t dstAddr, uint32_t size);
  uint32_t readReg(Channel channel, uint32_t regOffset);
  void writeReg(Channel channel, uint32_t regOffset, uint32_t value);
  void shutdown();
}

#endif // SUPERPS2_DMA_H
