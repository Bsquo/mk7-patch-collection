.include "macros.s"

.arm
.text

HOOK hook_ultraminiturbo_init, ultraMiniturbo_init, \
    add sp, sp, #0x24

HOOK hook_ultraminiturbo_calc, ultraMiniturbo_calc, \
    nop

HOOK hook_ultraminiturbo_update_dash_param, ultraMiniturbo_updateDashParam, \
    bics r0, r7, r8
