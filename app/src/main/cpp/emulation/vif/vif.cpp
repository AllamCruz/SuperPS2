#include "vif.h"
#include "../memory/memory.h"
#include "../../log.h"
#include <vector>
#include <cstring>

namespace vif {
  constexpr uint32_t QWC_SIZE = 16;

  bool init() {
    LOGI("VIF initialized.");
    return true;
  }

  #include "gif/gif.h"
void process() {
    gif::submitPacket(packet, sizeInQwords);
  }

  void process(dma::Channel channel) {
    using namespace dma;

    uint32_t SAR = readReg(channel, 0x00);
    uint32_t QWC = 1;  // Stub: quantidade fixa
    if (!memory::isValidAddress(SAR)) {
      LOGE("VIF process error: invalid SAR address.");
      return;
    }

    const uint32_t size = QWC * QWC_SIZE;
    const uint8_t* src = memory::getPointer(SAR);
    if (!src) {
      LOGE("VIF process error: null memory pointer.");
      return;
    }

    std::vector<uint32_t> packet(4 * 4); // Stub: 4 QWORDs = 64 bytes
    std::memcpy(packet.data(), src, size);
    sendToGif(packet.data(), QWC * 1);  // stub: 1 QWC

    writeReg(channel, 0x08, readReg(channel, 0x08) | 0x100); // CHCR.complete

    LOGI("VIF process complete on channel %d, QWC=%u", channel, QWC);
  }

  void shutdown() {
    LOGI("VIF shutdown.");
  }
}
