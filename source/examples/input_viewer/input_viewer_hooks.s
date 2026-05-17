.include "macros.s"

.arm
.text

HOOK hook_change_chara_wifi_initControl, changeCharaWifi_initControl, \
    add sp, sp, #0x11C

HOOK hook_change_chara_wifi_onPageEnter, changeCharaWifi_onPageEnter, \
    bx lr   @ Original instruction is `nop`, so we can proceed just fine from here
