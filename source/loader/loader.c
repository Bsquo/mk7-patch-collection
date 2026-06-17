/*
 * This file is a modified version of a file originally written by RicBent
 * for the program Magikoopa. Modified to fit alongside libctru in projects.
 */

#include <3ds/svc.h>
#include <mem.h>
#include "newcodeinfo.h"

extern u32 __bss_start;
extern u32 __bss_end;

Result svcOpenProcess(Handle* process, u32 processId);
Result svcGetProcessId(u32* out, Handle handle);
void svcBreak(UserBreakType breakReason);
Result svcControlProcessMemory(Handle process, u32 addr0, u32 addr1, u32 size, u32 type, u32 perm);

void loader_main(void) __attribute__((section(".loader")));
Handle getCurrentProcessHandle(void) __attribute__((section(".loader")));

void loader_main(void) {
  Result res;

  u32 address = NEWCODE_OFFSET;
  u32 neededMemory = ALIGN_UP(NEWCODE_SIZE, 0x1000);

  res = svcControlProcessMemory(getCurrentProcessHandle(), address, address, neededMemory, MEMOP_PROT, (MEMPERM_READ | MEMPERM_WRITE | MEMPERM_EXECUTE));

  if (res < 0) {
    svcBreak(USERBREAK_ASSERT);
  }

  // Clear .bss section
  __rt_memclr_w(&__bss_start, (u32) &__bss_end - (u32) &__bss_start);
}

Handle getCurrentProcessHandle(void) {
  Handle handle = 0;
  u32 currentPid = 0;
  Result res;

  svcGetProcessId(&currentPid, CUR_PROCESS_HANDLE);
  res = svcOpenProcess(&handle, currentPid);

  if (res != 0)
    return 0;

  return handle;
}