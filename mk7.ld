OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(loader_patch) /* Go to `patch_loader` in `patches.s` */

/* Addresses come from a version-specific .ld included via -T in the Makefile */
SECTIONS {

  . = 0x100000;

	/* Loader-related */
  .patch_loader 0x100000 : {
    *(.patch_loader)
  }

  . = 0x100018;

  # TODO: Do the same for global destructors
  .patch_call_global_constructors 0x100018 : {
    *(.patch_call_global_constructors)
  }

  /* --- Our custom code patches --- */
  /* show_timer_in_gp */
  .patch_show_timer_in_gp ADDR_SHOW_TIMER_IN_GP : {
    *(.patch_show_timer_in_gp)
  }
  .patch_set_time_control_pos ADDR_PATCH_SET_TIME_CONTROL_POS : {
    *(.patch_set_time_control_pos)
  }

  /* race_prints */
  .patch_race_prints_init ADDR_RACE_PRINTS_INIT : {
    *(.patch_race_prints_init)
  }
  .patch_race_prints_calc ADDR_RACE_PRINTS_CALC : {
    *(.patch_race_prints_calc)
  }

  /* change_chara_wifi */
  .patch_change_chara_wifi_initControl ADDR_CHANGE_CHARA_WIFI_INIT_CONTROL : {
    *(.patch_change_chara_wifi_initControl)
  }
  .patch_change_chara_wifi_onPageEnter ADDR_CHANGE_CHARA_WIFI_ON_PAGE_ENTER : {
    *(.patch_change_chara_wifi_onPageEnter)
  }

  /* class_lr_select */
  .patch_class_lr_select_initControl ADDR_CLASS_LR_SELECT_INIT_CONTROL : {
    *(.patch_class_lr_select_initControl)
  }
  .patch_class_lr_select_onPagePreStep ADDR_CLASS_LR_SELECT_ON_PAGE_PRE_STEP : {
    *(.patch_class_lr_select_onPagePreStep)
  }
  .patch_class_lr_select_onPageEnter ADDR_CLASS_LR_SELECT_ON_PAGE_ENTER : {
    *(.patch_class_lr_select_onPageEnter)
  }
  .patch_class_lr_select_onPageComplete ADDR_CLASS_LR_SELECT_ON_PAGE_COMPLETE : {
    *(.patch_class_lr_select_onPageComplete)
  }
  .patch_class_lr_select_onSliderSetting ADDR_CLASS_LR_SELECT_ON_SLIDER_SETTING : {
    *(.patch_class_lr_select_onSliderSetting)
  }
  .patch_class_lr_select_procOpenMenu ADDR_CLASS_LR_SELECT_PROC_OPEN_MENU : {
    *(.patch_class_lr_select_procOpenMenu)
  }

  /* ultra_miniturbo */
  .patch_ultraminiturbo_init ADDR_ULTRA_MINITURBO_INIT : {
    *(.patch_ultraminiturbo_init)
  }
  .patch_ultraminiturbo_calc ADDR_ULTRA_MINITURBO_CALC : {
    *(.patch_ultraminiturbo_calc)
  }
  .patch_ultraminiturbo_update_dash_param ADDR_ULTRA_MINITURBO_UPDATE_DASH_PARAM : {
    *(.patch_ultraminiturbo_update_dash_param)
  }

  /* We'll insert the loader's code at the very end of the executable's .text section */
  /* NOTE: This address depends on the game version! */
  . = ADDR_LOADER;
	/* Addr already 4 byte aligned! */
	/* . = ALIGN(4); */
	.loader : {
		*(.loader*)
	}

  .init : {
    *(.init)
  }

  .init_array : {
    __init_array_start = . ;
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    . = ALIGN(4);
    __init_array_end = . ;
  }

  .fini : {
    *(.fini)
  }

  .fini_array : {
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array))
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
      *(.eh_frame*)
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