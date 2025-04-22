#include "gs_packet_decoder.h"
#include <android/log.h>

#define TAG "GSDecoder"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

namespace gs {

std::string registerName(uint8_t regIndex) {
    switch (regIndex) {
        case PRIM: return "PRIM";
        case RGBAQ: return "RGBAQ";
        case ST: return "ST";
        case UV: return "UV";
        case XYZF2: return "XYZF2";
        case XYZ2: return "XYZ2";
        case TEX0_1: return "TEX0_1";
        case TEX0_2: return "TEX0_2";
        case CLAMP_1: return "CLAMP_1";
        case CLAMP_2: return "CLAMP_2";
        case FOG: return "FOG";
        case A_D: return "A+D";
        default: return "UNKNOWN";
    }
}

void decodeRegister(uint8_t regIndex, uint64_t data) {
    std::string reg = registerName(regIndex);
    LOGI("Decodificando [%s] = 0x%016llX", reg.c_str(), data);

    // Exemplo de interpretação básica de PRIM
    if (regIndex == PRIM) {
        uint8_t prim_type = data & 0x07;
        LOGI("PRIM: tipo primitiva = %d", prim_type); // 0=point, 1=line, etc.
    }

    // Mais decodificações específicas podem ser adicionadas aqui futuramente
}

} 