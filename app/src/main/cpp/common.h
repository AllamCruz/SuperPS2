#pragma once

#include <stdint.h>
#include <cstddef>
#include <limits>
#include <cstdint>

// Definições de tipos comuns
using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::size_t;
using std::ptrdiff_t;
using std::max_align_t;

// Definições para limites
#ifndef INT8_MIN
#define INT8_MIN std::numeric_limits<int8_t>::min()
#endif
#ifndef INT8_MAX
#define INT8_MAX std::numeric_limits<int8_t>::max()
#endif
#ifndef UINT8_MAX
#define UINT8_MAX std::numeric_limits<uint8_t>::max()
#endif

#ifndef INT16_MIN
#define INT16_MIN std::numeric_limits<int16_t>::min()
#endif
#ifndef INT16_MAX
#define INT16_MAX std::numeric_limits<int16_t>::max()
#endif
#ifndef UINT16_MAX
#define UINT16_MAX std::numeric_limits<uint16_t>::max()
#endif

#ifndef INT32_MIN
#define INT32_MIN std::numeric_limits<int32_t>::min()
#endif
#ifndef INT32_MAX
#define INT32_MAX std::numeric_limits<int32_t>::max()
#endif
#ifndef UINT32_MAX
#define UINT32_MAX std::numeric_limits<uint32_t>::max()
#endif

#ifndef INT64_MIN
#define INT64_MIN std::numeric_limits<int64_t>::min()
#endif
#ifndef INT64_MAX
#define INT64_MAX std::numeric_limits<int64_t>::max()
#endif
#ifndef UINT64_MAX
#define UINT64_MAX std::numeric_limits<uint64_t>::max()
#endif

// Definições para JNI
#define JNIEXPORT __attribute__((visibility("default")))
#define JNICALL 

// Macros úteis
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define UNUSED(x) (void)(x)
#define ALIGN(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

// Constantes comuns
constexpr uint32_t KB = 1024;
constexpr uint32_t MB = 1024 * KB;
constexpr uint32_t GB = 1024 * MB;

// Definições de tipos para PS2
using ps2_uint8 = uint8_t;
using ps2_uint16 = uint16_t;
using ps2_uint32 = uint32_t;
using ps2_uint64 = uint64_t;
using ps2_int8 = int8_t;
using ps2_int16 = int16_t;
using ps2_int32 = int32_t;
using ps2_int64 = int64_t; 