#include "pc.h"
#include <android/log.h>
#include <stdexcept>

#define TAG "PC"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

namespace ee {
namespace cpu {

static uint32_t programCounter = PC_INITIAL;
static uint32_t exceptionHandler = 0x80000000;  // Default exception handler address

bool initPC() {
    programCounter = PC_INITIAL;
    LOGI("Program Counter (PC) iniciado em 0x%08X", programCounter);
    return true;
}

void resetPC() {
    programCounter = PC_INITIAL;
    LOGI("Program Counter (PC) resetado para 0x%08X", programCounter);
}

void shutdownPC() {
    LOGI("Program Counter (PC) finalizado.");
}

void setPC(uint32_t value) {
    if (value % PC_ALIGNMENT != 0) {
        LOGE("PC set to unaligned address: 0x%08X", value);
        throw std::runtime_error("Unaligned PC address");
    }
    programCounter = value;
    LOGI("PC set to 0x%08X", programCounter);
}

uint32_t getPC() {
    return programCounter;
}

void advancePC() {
    programCounter += PC_ALIGNMENT;
}

void branchPC(int32_t offset) {
    // Calculate new PC with offset (offset is in words, so multiply by 4)
    uint32_t newPC = programCounter + (offset * PC_ALIGNMENT);
    if (newPC % PC_ALIGNMENT != 0) {
        LOGE("Branch to unaligned address: 0x%08X", newPC);
        throw std::runtime_error("Unaligned branch target");
    }
    programCounter = newPC;
    LOGI("Branch to 0x%08X", programCounter);
}

void jumpPC(uint32_t target) {
    if (target % PC_ALIGNMENT != 0) {
        LOGE("Jump to unaligned address: 0x%08X", target);
        throw std::runtime_error("Unaligned jump target");
    }
    programCounter = target;
    LOGI("Jump to 0x%08X", programCounter);
}

void jumpRegister(uint32_t address) {
    if (address % PC_ALIGNMENT != 0) {
        LOGE("Jump to unaligned register address: 0x%08X", address);
        throw std::runtime_error("Unaligned register jump target");
    }
    programCounter = address;
    LOGI("Jump to register address 0x%08X", programCounter);
}

void setExceptionPC(uint32_t handler) {
    if (handler % PC_ALIGNMENT != 0) {
        LOGE("Exception handler at unaligned address: 0x%08X", handler);
        throw std::runtime_error("Unaligned exception handler");
    }
    exceptionHandler = handler;
    LOGI("Exception handler set to 0x%08X", exceptionHandler);
}

uint32_t getExceptionPC() {
    return exceptionHandler;
}

void saveState(std::ostream& stream) {
    stream.write(reinterpret_cast<const char*>(&programCounter), sizeof(programCounter));
    stream.write(reinterpret_cast<const char*>(&exceptionHandler), sizeof(exceptionHandler));
}

void loadState(std::istream& stream) {
    stream.read(reinterpret_cast<char*>(&programCounter), sizeof(programCounter));
    stream.read(reinterpret_cast<char*>(&exceptionHandler), sizeof(exceptionHandler));
    
    // Validate loaded values
    if (programCounter % PC_ALIGNMENT != 0) {
        LOGE("Loaded unaligned PC: 0x%08X", programCounter);
        throw std::runtime_error("Invalid PC state loaded");
    }
    if (exceptionHandler % PC_ALIGNMENT != 0) {
        LOGE("Loaded unaligned exception handler: 0x%08X", exceptionHandler);
        throw std::runtime_error("Invalid exception handler state loaded");
    }
}

} // namespace cpu
} // namespace ee 