.include "macros.s"

.arm

PATCH_BL patch_race_prints_init, hook_race_prints_init
PATCH_BL patch_race_prints_calc, hook_race_prints_calc
