#pragma once

#include "common.hpp"
#include "versions.h"
#include <3ds/types.h>

// This macro ensures that functions written in C / C++ are never inlined
// Besides, in order to call our hook functions from the assembly,
// we use `extern "C"` so that the function name doesn't get mangled
// and thus it's easier to write the symbol in the assembly.
#if defined(__cplusplus)
    #define HOOK extern "C" __attribute__((noinline))
#else
    #define HOOK __attribute__((noinline))
#endif

#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof((arr)[0]))

// ASM-related macros

// Copies the value of a general-purpose register (`REG`)
// into a C / C++ variable (`OUT_VAR`)
#define READ_ARM_REG(REG, OUT_VAR) \
    __asm__ volatile ("mov %0, " #REG : "=r" (OUT_VAR))

#define PATCH_INSTR(addr, instr) do { \
    volatile uint32_t* patch_ptr = reinterpret_cast<volatile uint32_t*>(addr); \
    *patch_ptr = static_cast<uint32_t>(instr); \
} while(0)

// ARM thumb mode (16-bit):  
#define PATCH_THUMB16(addr, instr16) \
    *reinterpret_cast<volatile uint16_t*>(addr) = static_cast<uint16_t>(instr16)
