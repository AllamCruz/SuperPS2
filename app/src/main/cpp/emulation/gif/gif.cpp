#include "gif.h"
#include "../renderer/ps2renderer.h"
#include "../utils/log.h"
#include <vector>
#include <memory>

namespace {
    // Maximum number of GS commands we'll buffer before submitting
    constexpr size_t MAX_COMMAND_BUFFER = 1024;
    
    // Internal command buffer
    std::vector<gif::GSCommand> commandBuffer;
    
    // Current primitive state
    struct {
        uint32_t type;
        bool active;
    } currentPrim;

    // GIF tag structure (128 bits)
    struct GIFTag {
        uint64_t nloop : 15;
        uint64_t eop   : 1;
        uint64_t pad1  : 16;
        uint64_t id    : 14;
        uint64_t pre   : 1;
        uint64_t prim  : 11;
        uint64_t flg   : 2;
        uint64_t nreg  : 4;
        uint64_t reg   : 64;
    };

    void flushCommandBuffer() {
        if (!commandBuffer.empty()) {
            ps2renderer::submitGSCommands(commandBuffer.data(), commandBuffer.size());
            commandBuffer.clear();
        }
    }
}

namespace gif {

    bool init() {
    LOGI("Initializing GIF subsystem...");
    
    // Reserve space for command buffer
    commandBuffer.reserve(MAX_COMMAND_BUFFER);
    
    // Reset primitive state
    currentPrim.active = false;
    currentPrim.type = 0;
    
    LOGI("GIF initialized.");
        return true;
    }

    void submitPacket(const uint32_t* qwords, uint32_t sizeInQwords) {
    LOGI("Processing GIF packet: %u QWORDs", sizeInQwords);
    
    uint32_t processedQwords = 0;
    uint32_t generatedCommands = 0;
    
    while (processedQwords < sizeInQwords) {
        // Cast current QWORD to GIFTag
        const GIFTag* tag = reinterpret_cast<const GIFTag*>(&qwords[processedQwords * 2]);
        processedQwords++;
        
        // Validate tag kind
        if (tag->flg > 3) {
            LOGE("Invalid GIF tag kind: %u", tag->flg);
            return;
        }
        
        const TagKind kind = static_cast<TagKind>(tag->flg);
        const uint32_t nloop = tag->nloop;
        const uint32_t nreg = tag->nreg;
        
        // Process data based on tag kind
        switch (kind) {
            case TagKind::PACKED: {
                for (uint32_t loop = 0; loop < nloop; loop++) {
                    for (uint32_t reg = 0; reg < nreg; reg++) {
                        if (processedQwords >= sizeInQwords) {
                            LOGE("Packet ended unexpectedly");
                return;
            }

                        // Extract register ID from tag's reg field
                        uint32_t regID = (tag->reg >> (reg * 4)) & 0xF;
                        
                        // Add command to buffer
                        GSCommand cmd;
                        cmd.regID = regID;
                        cmd.value = *reinterpret_cast<const uint64_t*>(&qwords[processedQwords * 2]);
                        
                        commandBuffer.push_back(cmd);
                        generatedCommands++;
                        
                        // Handle special registers
                        if (regID == gs::REG_PRIM) {
                            currentPrim.type = cmd.value & 0x7;
                            currentPrim.active = true;
                            flushCommandBuffer();
                        }
                        
                        processedQwords++;
                    }
                }
                break;
            }
            
            case TagKind::REGLIST:
            case TagKind::IMAGE:
            case TagKind::DIRECT:
                // TODO: Implement other tag kinds
                LOGE("Unimplemented GIF tag kind: %u", static_cast<uint32_t>(kind));
                break;
        }
        
        // If EOP flag is set, flush commands
        if (tag->eop) {
            flushCommandBuffer();
        }
    }
    
    LOGI("GIF packet processed: %u QWORDs, generated %u GS commands", 
         processedQwords, generatedCommands);
    }

    void shutdown() {
    LOGI("Shutting down GIF subsystem...");
    commandBuffer.clear();
    LOGI("GIF shutdown complete.");
}

} // namespace gif