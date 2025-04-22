#ifndef SUPERPS2_SPU2_H
#define SUPERPS2_SPU2_H

#include <stdint.h>

namespace spu2 {
    bool init(int32_t sampleRate, int32_t channels, int32_t bufferSizeInFrames);
    void writeSamples(const uint16_t* pcmData, int32_t numSamples);
    void shutdown();
}

#endif // SUPERPS2_SPU2_H