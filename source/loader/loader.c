/*
 * This file is a modified version of a file originally written by RicBent
 * for the program Magikoopa. Modified to fit alongside libctru in projects.
 */

#include <3ds/svc.h>
#include <mem.h>
#include "newcodeinfo.h"

extern u32 __bss_start;
extern u32 __bss_end;

extern u32 __game_image_start;
extern u32 __game_image_end;
extern u32 __game_text_start;
extern u32 __game_text_end;
extern u32 __game_data_start;
extern u32 __game_data_end;
extern u32 __game_rodata_start;
extern u32 __game_rodata_end;
extern u32 __game_bss_start;
extern u32 __game_bss_end;

Result svcOpenProcess(Handle* process, u32 processId);
Result svcGetProcessId(u32* out, Handle handle);
void svcBreak(UserBreakType breakReason);
Result svcControlProcessMemory(Handle process, u32 addr0, u32 addr1, u32 size, u32 type, u32 perm);

void mapMemoryRegion(u32 addr0, u32 size, u32 perm) __attribute__((section(".loader")));
void loader_main(void) __attribute__((section(".loader")));
Handle getCurrentProcessHandle(void) __attribute__((section(".loader")));



void mapMemoryRegion(u32 addr0, u32 size, u32 perm) {
  Result res = svcControlProcessMemory(getCurrentProcessHandle(), addr0, addr0, size, MEMOP_PROT, perm);

  if (res < 0) {
    svcBreak(USERBREAK_ASSERT);
  }
}

void loader_main(void) {
  // Map the memory of the mod, so that we can execute its code, and read / write from its data
  mapMemoryRegion(NEWCODE_OFFSET, ALIGN_UP(NEWCODE_SIZE, 0x1000), (MEMPERM_READ | MEMPERM_WRITE | MEMPERM_EXECUTE));

  // Make the entirety of the original game's code and data RWX, to allow on-the-fly patches
  // .text
  mapMemoryRegion((u32) &__game_text_start, ALIGN_UP((u32) &__game_text_end - (u32) &__game_text_start, 0x1000), (MEMPERM_READ | MEMPERM_WRITE | MEMPERM_EXECUTE));
  // .data
  mapMemoryRegion((u32) &__game_data_start, ALIGN_UP((u32) &__game_data_end - (u32) &__game_data_start, 0x1000), (MEMPERM_READ | MEMPERM_WRITE | MEMPERM_EXECUTE));
  // .rodata
  mapMemoryRegion((u32) &__game_rodata_start, ALIGN_UP((u32) &__game_rodata_end - (u32) &__game_rodata_start, 0x1000), (MEMPERM_READ | MEMPERM_WRITE | MEMPERM_EXECUTE));
  // .bss
  //mapMemoryRegion((u32) &__game_bss_start, ALIGN_UP((u32) &__game_bss_end - (u32) &__game_bss_start, 0x1000), (MEMPERM_READ | MEMPERM_WRITE | MEMPERM_EXECUTE));

  // Clear the mod's .bss section
  __rt_memclr_w(&__bss_start, &__bss_end - &__bss_start);
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