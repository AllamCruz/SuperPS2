#include "interrupt.h"
#include <bitset>
#include <android/log.h>

#define TAG "EE_INTERRUPT"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// Número total de fontes de interrupção no EE
constexpr int INTERRUPT_SOURCES = 32;

static std::bitset<INTERRUPT_SOURCES> pendingInterrupts;
static bool waitingForInterrupt = false;

namespace ee {
namespace interrupt {

bool init() {
    pendingInterrupts.reset();
    waitingForInterrupt = false;
    LOGI("Interrupt controller initialized.");
    return true;
}

void shutdown() {
    LOGI("Interrupt controller shutdown.");
}

void reset() {
    pendingInterrupts.reset();
    waitingForInterrupt = false;
    LOGI("Interrupt controller reset.");
}

void setInterruptWaiting(bool waiting) {
    waitingForInterrupt = waiting;
}

void request(uint32_t source) {
    if (source >= INTERRUPT_SOURCES) {
        LOGW("Interrupt request out of range: %u", source);
        return;
    }
    pendingInterrupts.set(source);
    LOGI("Interrupt requested: %u", source);
}

void acknowledge(uint32_t source) {
    if (source >= INTERRUPT_SOURCES) return;
    pendingInterrupts.reset(source);
    LOGI("Interrupt acknowledged: %u", source);
}

bool hasPending() {
    return pendingInterrupts.any();
}

void process() {
    if (!hasPending()) return;

    for (uint32_t i = 0; i < INTERRUPT_SOURCES; ++i) {
        if (pendingInterrupts.test(i)) {
            // Aqui chamar a rotina de tratamento real
            LOGI("Processing interrupt: %u", i);

            // Simulando que a interrupção foi tratada
            acknowledge(i);
        }
    }

    waitingForInterrupt = false;
}

} // namespace interrupt
} // namespace ee 