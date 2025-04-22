#include "cheat.h"
#include "../ps2core.h"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace cheat {
    static std::vector<Code> codes;
    static bool initialized = false;

    bool init() {
        if (initialized) return true;
        codes.clear();
        initialized = true;
        return true;
    }

    bool loadCheatFile(const std::string& filePath) {
        if (!initialized) return false;

        std::ifstream file(filePath);
        if (!file.is_open()) return false;

        std::string line;
        while (std::getline(file, line)) {
            // Remove espaços e caracteres especiais
            line.erase(std::remove_if(line.begin(), line.end(), 
                [](unsigned char c) { return std::isspace(c); }), line.end());

            // Ignora linhas vazias ou comentários
            if (line.empty() || line[0] == '#') continue;

            // Formato esperado: XXXXXXXX YYYYYYYY
            if (line.length() != 17) continue; // 8 + 1 + 8 caracteres

            try {
                Code code;
                code.address = std::stoul(line.substr(0, 8), nullptr, 16);
                code.value = std::stoul(line.substr(9, 8), nullptr, 16);
                code.enabled = true;
                codes.push_back(code);
            } catch (...) {
                continue;
            }
        }

        return !codes.empty();
    }

    const std::vector<Code>& getCodes() {
        return codes;
    }

    void setEnabled(size_t index, bool enabled) {
        if (index < codes.size()) {
            codes[index].enabled = enabled;
        }
    }

    void applyAll() {
        if (!initialized) return;

        for (const auto& code : codes) {
            if (code.enabled) {
                // Aplica o código na RAM do PS2
                ps2::write32(code.address, code.value);
            }
        }
    }

    void shutdown() {
        codes.clear();
        initialized = false;
    }
} 