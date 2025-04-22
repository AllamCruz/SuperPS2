#ifndef SUPERPS2_GIF_H
#define SUPERPS2_GIF_H

#include <cstdint>

// GS Register IDs
namespace gs {
    constexpr uint32_t REG_PRIM   = 0x00;
    constexpr uint32_t REG_RGBAQ  = 0x01;
    constexpr uint32_t REG_ST     = 0x02;
    constexpr uint32_t REG_UV     = 0x03;
    constexpr uint32_t REG_XYZF2  = 0x04;
    constexpr uint32_t REG_XYZ2   = 0x05;
    constexpr uint32_t REG_TEX0_1 = 0x06;
    constexpr uint32_t REG_TEX0_2 = 0x07;
    constexpr uint32_t REG_CLAMP_1= 0x08;
    constexpr uint32_t REG_CLAMP_2= 0x09;
    constexpr uint32_t REG_FOG    = 0x0A;
    // Add more register IDs as needed
}

namespace gif {
    // GIF tag kinds
    enum class TagKind {
        PACKED  = 0,
        REGLIST = 1,
        IMAGE   = 2,
        DIRECT  = 3
    };

    // GIF command structure
    struct GSCommand {
        uint32_t regID;
        uint64_t value;
    };

    /**
     * @brief Initializes the GIF subsystem
     * @return true if initialization successful, false otherwise
     */
    bool init();

    /**
     * @brief Processes a packet of QWORDs containing GIF commands
     * @param qwords Pointer to array of QWORDs (128-bit data units)
     * @param sizeInQwords Number of QWORDs in the packet
     */
    void submitPacket(const uint32_t* qwords, uint32_t sizeInQwords);

    /**
     * @brief Shuts down the GIF subsystem and frees resources
     */
    void shutdown();
}

#endif // SUPERPS2_GIF_H