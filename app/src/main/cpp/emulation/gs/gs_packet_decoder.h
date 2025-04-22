#ifndef SUPERPS2_GS_PACKET_DECODER_H
#define SUPERPS2_GS_PACKET_DECODER_H

#include <cstdint>
#include <string>

namespace gs {

    enum Register {
        PRIM = 0x00,
        RGBAQ = 0x01,
        ST = 0x02,
        UV = 0x03,
        XYZF2 = 0x04,
        XYZ2 = 0x05,
        TEX0_1 = 0x06,
        TEX0_2 = 0x07,
        CLAMP_1 = 0x08,
        CLAMP_2 = 0x09,
        FOG = 0x0A,
        A_D = 0x0E
    };

    void decodeRegister(uint8_t regIndex, uint64_t data);
    std::string registerName(uint8_t regIndex);

}

#endif //SUPERPS2_GS_PACKET_DECODER_H 