#pragma once

typedef signed char        int8;   // -128 to 127
typedef short int          int16;  // -32,768 to 32,767
typedef int                int32;  // -2,147,483,648 to 2,147,483,647
typedef long long          int64;  // -(2^63) to (2^63)-1
typedef unsigned char      uint8;  // 0 to 255
typedef unsigned short     uint16; // 0 to 65,535
typedef unsigned int       uint32; // 0 to 4,294,967,295
typedef unsigned long long uint64; // 0 to 2^64-1
typedef float              float32;
typedef double             float64;


// These macros must exactly match those in the Windows SDK's intsafe.h.
#if not defined INT8_MIN
#ifndef _MSC_VER
#define INT8_MIN         (-127 - 1)
#define INT16_MIN        (-32767 - 1)
#define INT32_MIN        (-2147483647 - 1)
#define INT64_MIN        (-9223372036854775807 - 1)
#define INT8_MAX         127
#define INT16_MAX        32767
#define INT32_MAX        2147483647
#define INT64_MAX        9223372036854775807
#define UINT8_MAX        0xff
#define UINT16_MAX       0xffff
#define UINT32_MAX       0xffffffff
#define UINT64_MAX       0xffffffffffffffff
#else
#define INT8_MIN         (-127i8 - 1)
#define INT16_MIN        (-32767i16 - 1)
#define INT32_MIN        (-2147483647i32 - 1)
#define INT64_MIN        (-9223372036854775807i64 - 1)
#define INT8_MAX         127i8
#define INT16_MAX        32767i16
#define INT32_MAX        2147483647i32
#define INT64_MAX        9223372036854775807i64
#define UINT8_MAX        0xffui8
#define UINT16_MAX       0xffffui16
#define UINT32_MAX       0xffffffffui32
#define UINT64_MAX       0xffffffffffffffffui64
#endif // _MSC_VER
#endif // INT8_MIN