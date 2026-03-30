.include "macros.s"

.arm
.text

HOOK hook_show_timer_in_gp, initTimeInGP, \
    cpy r4, r0
