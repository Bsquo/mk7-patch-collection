.include "macros.s"

.arm

PATCH_B patch_disable_voices_1, hook_disable_voices_1
PATCH_B patch_disable_voices_2, hook_disable_voices_2
PATCH_B patch_disable_voices_3, hook_disable_voices_3
PATCH_BL patch_disable_voices_4, hook_disable_voices_4
