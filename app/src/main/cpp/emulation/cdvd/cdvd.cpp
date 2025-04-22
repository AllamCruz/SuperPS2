#include "cdvd.h"
#include "../memory/mem_mainram.h"
#include <android/log.h>

#define TAG "CDVD"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

namespace cdvd {
    static Status status = Status::NoDisc;
    static std::vector<uint8_t> isoData;
    static uint32_t currentLBA = 0;

    bool init() {
        // isoData já carregada em RAM (ps2core::loadISO)
        isoData = std::vector<uint8_t>(ee::memory::getRawPointer(0),
                                       ee::memory::getRawPointer(0) + ee::memory::MAIN_RAM_SIZE);
        status = isoData.empty() ? Status::NoDisc : Status::Idle;
        LOGI("CDVD initialized, size=%zu bytes", isoData.size());
        return status != Status::NoDisc;
    }

    void update() {
        // Simples: nenhum estado transitório implementado
        if (status == Status::Seeking || status == Status::Reading) {
            status = Status::Idle;
        }
    }

    bool seek(uint32_t lba) {
        if (status == Status::NoDisc) return false;
        currentLBA = lba;
        status = Status::Seeking;
        LOGI("CDVD seek to LBA %u", lba);
        return true;
    }

    bool readSector(uint32_t lba, uint8_t* outBuffer) {
        if (status == Status::NoDisc) return false;
        size_t offset = static_cast<size_t>(lba) * 2048;
        if (offset + 2048 > isoData.size()) {
            status = Status::Error;
            LOGE("CDVD read out of range: LBA %u", lba);
            return false;
        }
        status = Status::Reading;
        memcpy(outBuffer, isoData.data() + offset, 2048);
        LOGI("CDVD read sector LBA %u", lba);
        return true;
    }

    Status getStatus() {
        return status;
    }

    void shutdown() {
        isoData.clear();
        status = Status::NoDisc;
        LOGI("CDVD shutdown.");
    }
} 