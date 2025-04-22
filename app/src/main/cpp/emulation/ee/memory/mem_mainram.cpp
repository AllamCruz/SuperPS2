#include "mem_mainram.h"
#include <cstring>
#include <sstream>
#include <iomanip>
#include <android/log.h>
#include <stdexcept>

#define TAG "MainRAM"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

namespace ee {
namespace memory {

static uint8_t* mainRAM = nullptr;
static std::vector<ProtectedRegion> protectedRegions;

bool initMainRAM() {
    mainRAM = new(std::nothrow) uint8_t[MAIN_RAM_SIZE];
    if (!mainRAM) {
        LOGE("Falha ao alocar memória RAM principal.");
        return false;
    }
    resetMainRAM();
    protectedRegions.clear();
    LOGI("Memória RAM principal alocada e inicializada.");
    return true;
}

void resetMainRAM() {
    if (mainRAM) {
        std::memset(mainRAM, 0, MAIN_RAM_SIZE);
        protectedRegions.clear();
        LOGI("RAM principal resetada.");
    }
}

void shutdownMainRAM() {
    if (mainRAM) {
        delete[] mainRAM;
        mainRAM = nullptr;
        protectedRegions.clear();
        LOGI("RAM principal liberada.");
    }
}

bool isValidAddress(uint32_t address) {
    return (address >= MAIN_RAM_START && address <= MAIN_RAM_END) ||
           (address >= MAIN_RAM_MIRROR1_START && address <= MAIN_RAM_MIRROR1_END) ||
           (address >= MAIN_RAM_MIRROR2_START && address <= MAIN_RAM_MIRROR2_END) ||
           (address >= MAIN_RAM_MIRROR3_START && address <= MAIN_RAM_MIRROR3_END);
}

uint32_t translateAddress(uint32_t address) {
    if (address >= MAIN_RAM_MIRROR1_START && address <= MAIN_RAM_MIRROR1_END) {
        return address - MAIN_RAM_MIRROR1_START;
    } else if (address >= MAIN_RAM_MIRROR2_START && address <= MAIN_RAM_MIRROR2_END) {
        return address - MAIN_RAM_MIRROR2_START;
    } else if (address >= MAIN_RAM_MIRROR3_START && address <= MAIN_RAM_MIRROR3_END) {
        return address - MAIN_RAM_MIRROR3_START;
    }
    return address;
}

bool isRegionProtected(uint32_t address, AccessType type) {
    for (const auto& region : protectedRegions) {
        if (address >= region.start && address <= region.end) {
            if (type == AccessType::WRITE && region.readOnly) {
                return true;
            }
            if (type == AccessType::EXECUTE) {
                return true; // All protected regions are non-executable
            }
        }
    }
    return false;
}

void protectRegion(uint32_t start, uint32_t end, bool readOnly) {
    protectedRegions.push_back({start, end, readOnly});
    LOGI("Região protegida: 0x%08X - 0x%08X (readOnly: %d)", start, end, readOnly);
}

void unprotectRegion(uint32_t start, uint32_t end) {
    protectedRegions.erase(
        std::remove_if(protectedRegions.begin(), protectedRegions.end(),
            [start, end](const auto& region) {
                return region.start == start && region.end == end;
            }),
        protectedRegions.end()
    );
    LOGI("Região desprotegida: 0x%08X - 0x%08X", start, end);
}

uint8_t read8(uint32_t address) {
    if (!isValidAddress(address) || isRegionProtected(address, AccessType::READ)) {
        return 0;
    }
    return mainRAM[translateAddress(address) & MAIN_RAM_MASK];
}

uint16_t read16(uint32_t address) {
    if (!isValidAddress(address) || isRegionProtected(address, AccessType::READ)) {
        return 0;
    }
    uint32_t physAddr = translateAddress(address) & MAIN_RAM_MASK;
    if (physAddr + 1 >= MAIN_RAM_SIZE) return 0;
    return (mainRAM[physAddr] | (mainRAM[physAddr + 1] << 8));
}

uint32_t read32(uint32_t address) {
    if (!isValidAddress(address) || isRegionProtected(address, AccessType::READ)) {
        return 0;
    }
    uint32_t physAddr = translateAddress(address) & MAIN_RAM_MASK;
    if (physAddr + 3 >= MAIN_RAM_SIZE) return 0;
    return (mainRAM[physAddr] | (mainRAM[physAddr + 1] << 8) |
            (mainRAM[physAddr + 2] << 16) | (mainRAM[physAddr + 3] << 24));
}

uint64_t read64(uint32_t address) {
    if (!isValidAddress(address) || isRegionProtected(address, AccessType::READ)) {
        return 0;
    }
    uint32_t physAddr = translateAddress(address) & MAIN_RAM_MASK;
    if (physAddr + 7 >= MAIN_RAM_SIZE) return 0;
    return ((uint64_t)read32(physAddr)) | ((uint64_t)read32(physAddr + 4) << 32);
}

void write8(uint32_t address, uint8_t value) {
    if (!isValidAddress(address) || isRegionProtected(address, AccessType::WRITE)) {
        return;
    }
    mainRAM[translateAddress(address) & MAIN_RAM_MASK] = value;
}

void write16(uint32_t address, uint16_t value) {
    if (!isValidAddress(address) || isRegionProtected(address, AccessType::WRITE)) {
        return;
    }
    uint32_t physAddr = translateAddress(address) & MAIN_RAM_MASK;
    if (physAddr + 1 >= MAIN_RAM_SIZE) return;
    mainRAM[physAddr] = value & 0xFF;
    mainRAM[physAddr + 1] = (value >> 8) & 0xFF;
}

void write32(uint32_t address, uint32_t value) {
    if (!isValidAddress(address) || isRegionProtected(address, AccessType::WRITE)) {
        return;
    }
    uint32_t physAddr = translateAddress(address) & MAIN_RAM_MASK;
    if (physAddr + 3 >= MAIN_RAM_SIZE) return;
    for (int i = 0; i < 4; ++i) {
        mainRAM[physAddr + i] = (value >> (i * 8)) & 0xFF;
    }
}

void write64(uint32_t address, uint64_t value) {
    if (!isValidAddress(address) || isRegionProtected(address, AccessType::WRITE)) {
        return;
    }
    write32(address, (uint32_t)(value & 0xFFFFFFFF));
    write32(address + 4, (uint32_t)(value >> 32));
}

uint8_t* getRawPointer(uint32_t address) {
    if (!isValidAddress(address)) {
        return nullptr;
    }
    return &mainRAM[translateAddress(address) & MAIN_RAM_MASK];
}

void memset(uint32_t address, uint8_t value, uint32_t size) {
    if (!isValidAddress(address) || !isValidAddress(address + size - 1)) {
        return;
    }
    uint32_t physAddr = translateAddress(address) & MAIN_RAM_MASK;
    std::memset(&mainRAM[physAddr], value, size);
}

void memcpy(uint32_t dest, uint32_t src, uint32_t size) {
    if (!isValidAddress(dest) || !isValidAddress(src) || 
        !isValidAddress(dest + size - 1) || !isValidAddress(src + size - 1)) {
        return;
    }
    uint32_t physDest = translateAddress(dest) & MAIN_RAM_MASK;
    uint32_t physSrc = translateAddress(src) & MAIN_RAM_MASK;
    std::memcpy(&mainRAM[physDest], &mainRAM[physSrc], size);
}

void saveState(std::ostream& stream) {
    stream.write(reinterpret_cast<const char*>(mainRAM), MAIN_RAM_SIZE);
}

void loadState(std::istream& stream) {
    stream.read(reinterpret_cast<char*>(mainRAM), MAIN_RAM_SIZE);
}

std::string dumpMemory(uint32_t address, uint32_t size) {
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');
    
    for (uint32_t i = 0; i < size; i += 16) {
        oss << "0x" << std::setw(8) << (address + i) << ": ";
        for (uint32_t j = 0; j < 16; ++j) {
            if (i + j < size) {
                oss << std::setw(2) << (int)read8(address + i + j) << " ";
            } else {
                oss << "   ";
            }
        }
        oss << "\n";
    }
    
    return oss.str();
}

} // namespace memory
} // namespace ee 