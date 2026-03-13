OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

/* Addresses come from a version-specific .ld included via -T in the Makefile */
SECTIONS {

  . = 0x100000;

  /* --- Our custom code patches --- */
  /* print_kart_info */
  .patch_print_kart_info ADDR_PRINT_KART_INFO : {
    *(.patch_print_kart_info)
  }

  /* Our custom code goes here */
  . = ADDR_CUSTOM_CODE;
  _LD_SYMBOL_TEXT_START = . ;
    .text : {
        __text_start = . ;
        *(.text*)
        *(.rodata*)
        *(.data*)
        *(.bss*)
        *(.init*)
        *(.fini*)
        *(.eh_frame*)
        *(.init_array*)
        *(.fini_array*)
        *(COMMON)
        __text_end  = . ;
  }
  _LD_SYMBOL_TEXT_SIZE = __text_end - __text_start;

  /* ARM unwind tables (required even without exceptions) */
  .ARM.extab : {
      *(.ARM.extab* .gnu.linkonce.armextab.*)
  }

  .ARM.exidx : {
      __exidx_start = .;
      *(.ARM.exidx* .gnu.linkonce.armexidx.*)
      __exidx_end = .;
  }

  __end__ = .;
  PROVIDE(end = .);
}