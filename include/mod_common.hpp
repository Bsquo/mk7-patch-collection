#pragma once

#include "addresses.hpp"
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

// Copies the value of a C / C++ variable (`OUT_VAR`)
// into a general-purpose register (`REG`)
#define WRITE_ARM_REG(REG, IN_VAR) \
    __asm__ volatile ("mov " #REG ", %0" :: "r" (IN_VAR))

// Patches a single instruction in the original executable's code
// sym: Address of the instruction
// instr: The encoded instruction
// Eg: PATCH_INSTR(0x0033ED5C, 0xE12FFF1E);
#define PATCH_INSTR(sym, instr) do { \
    *(vu32 *)(u32)(&(sym)) = (u32)(instr); \
} while(0)

// Writes an array of data into an address
// Eg: 
/*
const u32 patch[] = {
    0xE12FFF1E, // bx lr
    0xE3A00000, // mov r0, #0
    0xE12FFF1E  // bx lr
};

WRITE_BYTES(0x0033ED5C, patch);
*/
#define WRITE_BYTES(addr, src) \
    __rt_memcpy_w((void *)(addr), &(src), sizeof(src))

// Reads a single 32-bit instruction from memory
// Eg: u32 instr = READ_INSTR(0x0033ED5C);
#define READ_INSTR(addr) \
    (*(vu32 *)(addr))
