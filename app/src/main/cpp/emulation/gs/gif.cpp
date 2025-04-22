#include "gif.h"
#include "gs_packet_decoder.h"
#include "gs_pipeline.h"
#include <android/log.h>

#define TAG "GIF"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

namespace gs {

struct GifTag {
    uint64_t NLOOP : 15;
    uint64_t EOP : 1;
    uint64_t : 16;
    uint64_t PRE : 1;
    uint64_t PRIM : 11;
    uint64_t FLG : 2;
    uint64_t NREG : 4;
    uint64_t REGS : 64;
};

void reset() {
    LOGI("GIF reset");
}

// Interpreta um pacote de comandos do GIF
void processPacket(const uint64_t* data, size_t size) {
    if (size < 1) {
        LOGI("Pacote GIF vazio");
        return;
    }

    GifTag tag;
    memcpy(&tag, data, sizeof(GifTag));
    LOGI("GIF Tag: NLOOP=%u, FLG=%u, NREG=%u", tag.NLOOP, tag.FLG, tag.NREG);

    if (tag.FLG == 0) { // PACKED mode
        const uint64_t* regData = data + 1;
        size_t regsProcessed = 0;
        
        for (uint64_t i = 0; i < tag.NLOOP; i++) {
            for (uint64_t j = 0; j < tag.NREG; j++) {
                if (regsProcessed >= size - 1) {
                    LOGI("Pacote truncado");
                    return;
                }
                
                uint8_t regIndex = (tag.REGS >> (j * 8)) & 0xFF;
                g_pipeline.writeRegister(regIndex, regData[regsProcessed]);
                regsProcessed++;
            }
        }
    } else {
        // FLG 1 ou 2 = REGLIST / IMAGE (não implementado aqui)
        LOGI("GIF mode FLG=%d não suportado ainda", tag.FLG);
    }

    if (tag.EOP) {
        LOGI("GIF EOP (End of Packet)");
    }
}

} 