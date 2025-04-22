#pragma once

#include <cstdint>
#include <stdint.h>
#include <cstddef>

// Definições de tipos comuns
using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;

// Definições para JNI
#define JNIEXPORT __attribute__((visibility("default")))
#define JNICALL 