OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(loader_patch) /* Go to `patch_loader` in `patches.s` */

/* Addresses come from a version-specific .ld included via -T in the Makefile */
SECTIONS {

  . = 0x100000;

	/* Loader-related patches */
  .patch_loader 0x100000 : {
    *(.patch_loader)
  }

  /* --- Our custom code patches --- */
  /* show_timer_in_gp */
  .patch_show_timer_in_gp ADDR_SHOW_TIMER_IN_GP : {
    *(.patch_show_timer_in_gp)
  }

  /* change_chara_wifi */
  .patch_change_chara_wifi_initControl ADDR_CHANGE_CHARA_WIFI_INIT_CONTROL : {
    *(.patch_change_chara_wifi_initControl)
  }

  .patch_change_chara_wifi_onPageEnter ADDR_CHANGE_CHARA_WIFI_ON_PAGE_ENTER : {
    *(.patch_change_chara_wifi_onPageEnter)
  }

	/* We'll insert the loader's code at the very end of the executable's .text section */
  /* NOTE: This address depends on the game version! */
  . = ADDR_LOADER;
	/* Addr already 4 byte aligned! */
	/* . = ALIGN(4); */
	.loader : {
		*(.loader*)
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