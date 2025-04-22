#ifndef SUPERPS2_VIF_H
#define SUPERPS2_VIF_H

#include <cstdint>
#include "dma/dma.h"

namespace vif {
  bool init();
  void process(dma::Channel channel);
  void sendToGif(const uint32_t* packet, uint32_t sizeInQwords);
  void shutdown();
}

#endif // SUPERPS2_VIF_H
