#ifndef SUPERPS2_EE_CORE_H
#define SUPERPS2_EE_CORE_H

#include <cstdint>

namespace ee {

    void init();
    void reset();
    void runFrame();
    void shutdown();
    void printState();

}

#endif //SUPERPS2_EE_CORE_H