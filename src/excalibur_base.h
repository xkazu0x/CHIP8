#ifndef EXCALIBUR_BASE_H
#define EXCALIBUR_BASE_H

//////////////////////////////////
// NOTE(xkazu0x): context cracking

#if defined(__clang__)
#define COMPILER_CLANG 1

#if defined(_WIN32)
#define OS_WINDOWS 1
#elif defined(__gnu_linux__)
#define OS_LINUX 1
#elif defined(__APPLE__) && defined(__MACH__)
#define OS_MAC 1
#else
#error MISSING OPERATING SYSTEM DETECTION
#endif

#if defined(__amd64__)
#define ARCH_X64 1
// TODO(xkazu0x): verify this works on clang
#elif defined(__i386__)
#define ARCH_X86 1
// TODO(xkazu0x): verify this works on clang
#elif defined(__arm__)
#define ARCH_ARM 1
// TODO(xkazu0x): verify this works on clang
#elif defined(__aarch64__)
#define ARCH_ARM64 1
#else
#error MISSING ARCHITECTURE DETECTION
#endif

#elif defined(_MSC_VER)
#define COMPILER_CL 1

#if defined(_WIN32)
#define OS_WINDOWS 1
#else
#error MISSING OPERATING SYSTEM DETECTION
#endif

#if defined(_M_AMD64)
#define ARCH_X64 1
#elif defined(_M_I86)
#define ARCH_X86 1
#elif defined(_M_ARM)
#define ARCH_ARM 1
// TODO(xkazu0x): ARM64??
#else
#error MISSING ARCHITECTURE DETECTION
#endif

#elif defined(__GNUC__)
#define COMPILER_GCC 1

#if defined(_WIN32)
#define OS_WINDOWS 1
#elif defined(__gnu_linux__)
#define OS_LINUX 1
#elif defined(__APPLE__) && defined(__MACH__)
#define OS_MAC 1
#else
#error MISSING OPERATING SYSTEM DETECTION
#endif

#if defined(__amd64__)
#define ARCH_X64 1
#elif defined(__i386__)
#define ARCH_X86 1
#elif defined(__arm__)
#define ARCH_ARM 1
#elif defined(__aarch64__)
#define ARCH_ARM64 1
#else
#error MISSING ARCHITECTURE DETECTION
#endif

#else
#error NO CONTEXT CRACKING FOR THIS COMPILER
#endif

#if !defined(COMPILER_CL)
#define COMPILER_CL 0
#endif
#if !defined(COMPILER_CLANG)
#define COMPILER_CLANG 0
#endif
#if !defined(COMPILER_GCC)
#define COMPILER_GCC 0
#endif

#if !defined(OS_WINDOWS)
#define OS_WINDOWS 0
#endif
#if !defined(OS_LINUX)
#define OS_LINUX 0
#endif
#if !defined(OS_MAC)
#define OS_MAC 0
#endif

#if !defined(ARCH_X64)
#define ARCH_X64 0
#endif
#if !defined(ARCH_X86)
#define ARCH_X86 0
#endif
#if !defined(ARCH_ARM)
#define ARCH_ARM 0
#endif
#if !defined(ARCH_ARM64)
#define ARCH_ARM64 0
#endif

///////////////////////////////
// NOTE(xkazu0x): helper macros

#define EX_STMNT(x) do { x } while(0)
#define EX_ASSERT_BREAK() (*(int *)0 = 0)

#ifdef EXCALIBUR_DEBUG
# define EX_ASSERT(x) EX_STMNT(if (!(x)) { EX_ASSERT_BREAK(); })
#else
# define EX_ASSERT(x)
#endif

#define EX_KILOBYTES(x) ((x)*1024LL)
#define EX_MEGABYTES(x) (EX_KILOBYTES(x)*1024LL)
#define EX_GIGABYTES(x) (EX_MEGABYTES(x)*1024LL)
#define EX_TERABYTES(x) (EX_GIGABYTES(x)*1024LL)

#define EX_ARRAY_COUNT(x) (sizeof(x)/sizeof(*(x)))

#define EX_INT_FROM_PTR(p) (unsigned long long)((char*)p - (char*)0)
#define EX_PTR_FROM_INT(n) (void*)((char*)0 + (n))

#define EX_MEMBER(T, m) (((T*)0)->m)
#define EX_OFFSETOF(T, m) EX_INT_FROM_PTR(&EX_MEMBER(T, m))

#define EX_MIN(a, b) (((a)<(b))?(a):(b))
#define EX_MAX(a, b) (((a)>(b))?(a):(b))
#define EX_CLAMP(a, x, b) (((x)<(a))?(a):\
                           ((b)<(x))?(b):(x))
#define EX_CLAMP_TOP(a, b) EXM_MIN(a, b)
#define EX_CLAMP_BOT(a, b) EXM_MAX(a, b)

#define EX_FALSE 0
#define EX_TRUE 1

#define global static
#define local static
#define internal static

/////////////////////////////
// NOTE(xkazu0x): basic types
#include <stdint.h>

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef s8 b8;
typedef s16 b16;
typedef s32 b32;
typedef s64 b64;

typedef float f32;
typedef double f64;

/////////////////////////////////
// NOTE(xkazu0x): basic constants

global s8  s8_min  = (s8) 0x80;
global s16 s16_min = (s16)0x8000;
global s32 s32_min = (s32)0x80000000;
global s64 s64_min = (s64)0x8000000000000000llu;

global s8  s8_max  = (s8) 0x7F;
global s16 s16_max = (s16)0x7FFF;
global s32 s32_max = (s32)0x7FFFFFFF;
global s64 s64_max = (s64)0x7FFFFFFFFFFFFFFFllu;

global u8  u8_max  = 0xFF;
global u16 u16_max = 0xFFFF;
global u32 u32_max = 0xFFFFFFFF;
global u64 u64_max = 0xFFFFFFFFFFFFFFFFllu;

#include <float.h>
global f32 f32_max = FLT_MAX;
global f32 f32_min = -FLT_MAX;

global f32 pi32 = 3.14159265359f;
global f64 pi64 = 3.14159265359;

////////////////////////////////////
// NOTE(xkazu0x): symbolic constants

enum OPERATING_SYSTEM {
    OPERATING_SYSTEM_UNDEFINED,
    OPERATING_SYSTEM_WINDOWS,
    OPERATING_SYSTEM_LINUX,
    OPERATING_SYSTEM_MAC,
    OPERATING_SYSTEM_MAX,
};

enum ARCHITECTURE {
    ARCHITECTURE_UNDEFINED,
    ARCHITECTURE_X64,
    ARCHITECTURE_X86,
    ARCHITECTURE_ARM,
    ARCHITECTURE_ARM64,
    ARCHITECTURE_MAX,
};

/////////////////////////////////////////////
// NOTE(xkazu0x): symbolic constant functions

internal OPERATING_SYSTEM operating_system_from_context(void);
internal ARCHITECTURE architecture_from_context(void);

internal char *string_from_operating_system(OPERATING_SYSTEM os);
internal char *string_from_architecture(ARCHITECTURE arch);

#endif // EXCALIBUR_BASE_H
