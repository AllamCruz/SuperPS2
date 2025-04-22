#include "../vif/vif.h"
#include "dma.h"
#include "../memory/memory.h"
#include "../../log.h"
#include <vector>
#include <cstring>
#include <map>

namespace dma {
  constexpr uint32_t REG_SAR  = 0x00;
  constexpr uint32_t REG_SMR  = 0x04;
  constexpr uint32_t REG_CHCR = 0x08;

  struct DMAChannel {
    uint32_t SAR = 0;
    uint32_t SMR = 0;
    uint32_t CHCR = 0;
    std::vector<uint8_t> buffer;
  };

  std::map<Channel, DMAChannel> channels;

  bool init() {
    channels.clear();
    for (int i = 0; i <= 5; ++i) {
      channels[(Channel)i] = DMAChannel();
    }
    LOGI("DMA initialized.");
    return true;
  }

  bool transfer(Channel channel, uint32_t srcAddr, uint32_t dstAddr, uint32_t size) {
    if (!memory::isValidAddress(srcAddr) || !memory::isValidAddress(dstAddr)) {
      LOGE("DMA transfer error on channel %d: invalid address.", channel);
      return false;
    }

    auto& ch = channels[channel];
    ch.buffer.resize(size);
    std::memcpy(ch.buffer.data(), memory::getPointer(srcAddr), size);
    memory::writeBlock(dstAddr, ch.buffer.data(), size); // stub: simula escrita em periférico
    ch.CHCR &= ~0x1; // limpa bit start
    ch.CHCR |= 0x100; // seta bit complete
    LOGI("DMA transfer completed on channel %d.\n    if (channel == dma::Channel::VIF0 || channel == dma::Channel::VIF1) { vif::process(channel); }", channel);
    return true;
  }

  uint32_t readReg(Channel channel, uint32_t regOffset) {
    auto& ch = channels[channel];
    switch (regOffset) {
      case REG_SAR: return ch.SAR;
      case REG_SMR: return ch.SMR;
      case REG_CHCR: return ch.CHCR;
      default: return 0;
    }
  }

  void writeReg(Channel channel, uint32_t regOffset, uint32_t value) {
    auto& ch = channels[channel];
    switch (regOffset) {
      case REG_SAR: ch.SAR = value; break;
      case REG_SMR: ch.SMR = value; break;
      case REG_CHCR:
        ch.CHCR = value;
        if (value & 0x1) {
          // se bit "start" foi setado
          transfer(channel, ch.SAR, ch.SMR, 256); // stub: tamanho fixo
        }
        break;
    }
  }

  void shutdown() {
    channels.clear();
    LOGI("DMA shutdown.");
  }
}
