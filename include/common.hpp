#include <3ds/types.h>

// Copies the value of a general-purpose register (`REG`)
// into a C / C++ variable (`OUT_VAR`)
#define READ_ARM_REG(REG, OUT_VAR) \
    __asm__ volatile ("mov %0, " #REG : "=r" (OUT_VAR))

// This macro ensures that functions written in C / C++ are never inlined
// Besides, in order to call our hook functions from the assembly,
// we use `extern "C"` so that the function name doesn't get mangled
// and thus it's easier to write the symbol in the assembly.
#if defined(__cplusplus)
    #define HOOK extern "C" __attribute__((noinline))
#else
    #define HOOK __attribute__((noinline))
#endif
