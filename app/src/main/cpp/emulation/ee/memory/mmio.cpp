#include "mmio.h"
#include "../cdvd/cdvd.h"
#include "../gs/gs.h"

uint32_t read32(uint32_t addr) {
    // ... existing code ...
    
    // CDVD MMIO: 0x1F402000
    if (addr >= 0x1F402000 && addr < 0x1F403000) {
        cdvd::update();
        uint32_t offset = addr - 0x1F402000;
        switch (offset) {
            case 0x00: // Status
                return static_cast<uint32_t>(cdvd::getStatus());
            case 0x04: // LBA low
                return currentLBA & 0xFFFFFFFF;
            case 0x08: { // Read data
                static uint8_t sectorBuf[2048];
                if (cdvd::readSector(currentLBA, sectorBuf)) {
                    int idx = (addr - 0x1F402008) / 4;
                    uint32_t* words = reinterpret_cast<uint32_t*>(sectorBuf);
                    return words[idx];
                }
                return 0;
            }
            default:
                return 0;
        }
    }
    
    // GS MMIO range: 0x12000000 – 0x12000FFF
    if (addr >= 0x12000000 && addr < 0x12001000) {
        return gs::readRegister(addr - 0x12000000);
    }
    
    // ... existing code ...
}

void write32(uint32_t addr, uint32_t value) {
    // ... existing code ...
    
    // CDVD MMIO: 0x1F402000
    if (addr >= 0x1F402000 && addr < 0x1F403000) {
        uint32_t offset = addr - 0x1F402000;
        if (offset == 0x0C) {
            // Seek command: value contains LBA
            cdvd::seek(value);
        }
        return;
    }
    
    // GS MMIO range: 0x12000000 – 0x12000FFF
    if (addr >= 0x12000000 && addr < 0x12001000) {
        gs::writeRegister(addr - 0x12000000, value);
        return;
    }
    
    // ... existing code ...
} 