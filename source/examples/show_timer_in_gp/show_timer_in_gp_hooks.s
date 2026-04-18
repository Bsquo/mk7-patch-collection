.include "macros.s"

.arm
.text

HOOK hook_show_timer_in_gp, initTimeInGP, \
    cpy r4, r0

HOOK hook_set_time_control_pos, setTimeControlPos, \
    ldr r1, [r4, #0x3a0]
