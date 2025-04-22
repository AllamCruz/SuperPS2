#include "cop0.h"
#include <android/log.h>

#define LOG_TAG "COP0"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

namespace cop0 {
    uint32_t cop0Regs[32];

    void reset() {
        for (int i = 0; i < 32; ++i)
            cop0Regs[i] = 0;

        // Configura valores padrões típicos
        cop0Regs[Register::Status] = 0x00400000; // EXL=0, IE=0
        cop0Regs[Register::PRId] = 0x00020000;   // ID fictício do EE
    }

    uint32_t read(uint8_t reg) {
        LOGD("COP0 Read: R%d = 0x%08X", reg, cop0Regs[reg]);
        return cop0Regs[reg];
    }

    void write(uint8_t reg, uint32_t value) {
        // Protege PRId, Count, etc. se necessário
        if (reg == Register::PRId) return;

        cop0Regs[reg] = value;
        LOGD("COP0 Write: R%d = 0x%08X", reg, value);
    }
}
