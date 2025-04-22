#ifndef SUPERPS2_COP0_H
#define SUPERPS2_COP0_H

#include <cstdint>

namespace cop0 {
    enum Register {
        Index = 0,
        Random = 1,
        EntryLo0 = 2,
        EntryLo1 = 3,
        Context = 4,
        PageMask = 5,
        Wired = 6,
        BadVAddr = 8,
        Count = 9,
        EntryHi = 10,
        Compare = 11,
        Status = 12,
        Cause = 13,
        EPC = 14,
        PRId = 15,
        Config = 16,
        LLAddr = 17,
        WatchLo = 18,
        WatchHi = 19,
        XContext = 20
        // ... demais se necessário
    };

    void reset();
    uint32_t read(uint8_t reg);
    void write(uint8_t reg, uint32_t value);

    extern uint32_t cop0Regs[32];
}

#endif
