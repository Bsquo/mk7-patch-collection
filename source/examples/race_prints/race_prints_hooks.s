.include "macros.s"

.arm
.text

HOOK hook_race_prints_init, racePrints_init, \
    cpy r4, r0

HOOK hook_race_prints_calc, racePrints_calc, \
    cpy r4, r0
