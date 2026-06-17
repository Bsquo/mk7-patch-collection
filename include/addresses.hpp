#pragma once

#include <3ds/types.h>

// If you need to use any symbol from `addresses.ld` in C / C++ code,
// put those here.

extern "C" {
    extern const u32 ADDR_DISABLE_CPU_RUBBERBANDING;
    extern const u32 ADDR_DISABLE_BGM_1;
    extern const u32 ADDR_DISABLE_BGM_2;
    extern const u32 ADDR_DISABLE_BGM_3;
    extern const u32 ADDR_DISABLE_BGM_4;
}