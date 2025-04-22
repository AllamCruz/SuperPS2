#include "ram.h"
#include <cstring>
#include <vector>
#include <android/log.h>

#define TAG "EE_RAM"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

namespace ee {
namespace memory {

static uint8_t* ram = nullptr;
static std::vector<std::pair<uint32_t, uint32_t>> protectedRegions;

bool init() {
    ram = new(std::nothrow) uint8_t[EE_RAM_SIZE];
    if (!ram) {
        LOGE("Falha ao alocar memória principal EE.");
        return false;
    }
    std::memset(ram, 0, EE_RAM_SIZE);
    protectedRegions.clear();
    LOGI("Memória principal do EE alocada (%u bytes).", EE_RAM_SIZE);
    return true;
}

void reset() {
    if (ram) {
        std::memset(ram, 0, EE_RAM_SIZE);
    }
    protectedRegions.clear();
    LOGI("Memória principal do EE resetada.");
}

void shutdown() {
    delete[] ram;
    ram = nullptr;
    protectedRegions.clear();
    LOGI("Memória principal do EE liberada.");
}

bool isAddressValid(uint32_t addr) {
    // Check if address is in any valid RAM region or mirror
    return (addr >= EE_RAM_START && addr <= EE_RAM_END) ||
           (addr >= EE_RAM_MIRROR1_START && addr <= EE_RAM_MIRROR1_END) ||
           (addr >= EE_RAM_MIRROR2_START && addr <= EE_RAM_MIRROR2_END) ||
           (addr >= EE_RAM_MIRROR3_START && addr <= EE_RAM_MIRROR3_END);
}

uint32_t translateAddress(uint32_t addr) {
    // Translate mirrored addresses to physical RAM
    if (addr >= EE_RAM_MIRROR1_START && addr <= EE_RAM_MIRROR1_END) {
        return addr - EE_RAM_MIRROR1_START;
    } else if (addr >= EE_RAM_MIRROR2_START && addr <= EE_RAM_MIRROR2_END) {
        return addr - EE_RAM_MIRROR2_START;
    } else if (addr >= EE_RAM_MIRROR3_START && addr <= EE_RAM_MIRROR3_END) {
        return addr - EE_RAM_MIRROR3_START;
    }
    return addr;
}

bool isRegionProtected(uint32_t addr) {
    for (const auto& region : protectedRegions) {
        if (addr >= region.first && addr <= region.second) {
            return true;
        }
    }
    return false;
}

void protectRegion(uint32_t start, uint32_t end) {
    protectedRegions.emplace_back(start, end);
    LOGI("Região protegida: 0x%08X - 0x%08X", start, end);
}

void unprotectRegion(uint32_t start, uint32_t end) {
    protectedRegions.erase(
        std::remove_if(protectedRegions.begin(), protectedRegions.end(),
            [start, end](const auto& region) {
                return region.first == start && region.second == end;
            }),
        protectedRegions.end()
    );
    LOGI("Região desprotegida: 0x%08X - 0x%08X", start, end);
}

uint8_t read8(uint32_t addr) {
    if (!isAddressValid(addr) || isRegionProtected(addr)) return 0;
    return ram[translateAddress(addr) & EE_RAM_MASK];
}

uint16_t read16(uint32_t addr) {
    if (!isAddressValid(addr) || isRegionProtected(addr)) return 0;
    uint32_t physAddr = translateAddress(addr) & EE_RAM_MASK;
    if (physAddr + 1 >= EE_RAM_SIZE) return 0;
    return (ram[physAddr] | (ram[physAddr + 1] << 8));
}

uint32_t read32(uint32_t addr) {
    if (!isAddressValid(addr) || isRegionProtected(addr)) return 0;
    uint32_t physAddr = translateAddress(addr) & EE_RAM_MASK;
    if (physAddr + 3 >= EE_RAM_SIZE) return 0;
    return (ram[physAddr] | (ram[physAddr + 1] << 8) | 
            (ram[physAddr + 2] << 16) | (ram[physAddr + 3] << 24));
}

uint64_t read64(uint32_t addr) {
    if (!isAddressValid(addr) || isRegionProtected(addr)) return 0;
    uint32_t physAddr = translateAddress(addr) & EE_RAM_MASK;
    if (physAddr + 7 >= EE_RAM_SIZE) return 0;
    return ((uint64_t)read32(physAddr)) | ((uint64_t)read32(physAddr + 4) << 32);
}

void write8(uint32_t addr, uint8_t value) {
    if (!isAddressValid(addr) || isRegionProtected(addr)) return;
    ram[translateAddress(addr) & EE_RAM_MASK] = value;
}

void write16(uint32_t addr, uint16_t value) {
    if (!isAddressValid(addr) || isRegionProtected(addr)) return;
    uint32_t physAddr = translateAddress(addr) & EE_RAM_MASK;
    if (physAddr + 1 >= EE_RAM_SIZE) return;
    ram[physAddr] = value & 0xFF;
    ram[physAddr + 1] = (value >> 8) & 0xFF;
}

void write32(uint32_t addr, uint32_t value) {
    if (!isAddressValid(addr) || isRegionProtected(addr)) return;
    uint32_t physAddr = translateAddress(addr) & EE_RAM_MASK;
    if (physAddr + 3 >= EE_RAM_SIZE) return;
    for (int i = 0; i < 4; i++) {
        ram[physAddr + i] = (value >> (8 * i)) & 0xFF;
    }
}

void write64(uint32_t addr, uint64_t value) {
    if (!isAddressValid(addr) || isRegionProtected(addr)) return;
    write32(addr, value & 0xFFFFFFFF);
    write32(addr + 4, (value >> 32) & 0xFFFFFFFF);
}

uint8_t* getRawMemory() {
    return ram;
}

void invalidateICache() {
    // TODO: Implement instruction cache invalidation
}

void invalidateDCache() {
    // TODO: Implement data cache invalidation
}

void writebackDCache() {
    // TODO: Implement data cache writeback
}

} // namespace memory
} // namespace ee 