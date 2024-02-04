#pragma once

#include <cinttypes>

#define compressed __attribute__((packed))

typedef std::int8_t int8;
typedef std::int16_t int16;
typedef std::int32_t int32;
typedef std::int64_t int64;
typedef std::uint8_t uint8;
typedef std::uint16_t uint16;
typedef std::uint32_t uint32;
typedef std::uint64_t uint64;

typedef float float32;
typedef double float64;
// NOT IMPLEMENTED typedef long double float128;

// DEFAULT typedef char char;
typedef std::byte byte;
