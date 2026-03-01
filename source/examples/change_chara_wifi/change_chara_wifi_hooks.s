.arm
.text

.global hook_change_chara_wifi_initControl
hook_change_chara_wifi_initControl:
    push {r0-r12, lr}
    bl changeCharaWifi_initControl
    pop  {r0-r12, lr}
    add sp, sp, #0x11C        @ Original instruction
    bx lr

.global hook_change_chara_wifi_onPageEnter
hook_change_chara_wifi_onPageEnter:
    push {r0-r12, lr}
    bl changeCharaWifi_onPageEnter
    pop  {r0-r12, lr}
    @ Original instruction is `nop`, so we can proceed just fine from here
    bx lr

