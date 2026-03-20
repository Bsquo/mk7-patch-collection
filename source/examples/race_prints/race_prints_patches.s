.arm

.section .patch_race_prints_init
.global patch_race_prints_init
patch_race_prints_init:
    bl hook_race_prints_init

.section .patch_race_prints_calc
.global patch_race_prints_calc
patch_race_prints_calc:
    bl hook_race_prints_calc
