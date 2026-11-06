.include "macros.s"

.arm

PATCH_ASM patch_enable_unused_demo_cameras_goal_1, \
    str r0,[r4, #0x5c]

PATCH_ASM patch_enable_unused_demo_cameras_goal_2, \
    mov r8, #0x0

PATCH_BL patch_enable_unused_demo_cameras_goal_3, hook_enable_unused_demo_cameras_goal

PATCH_BL patch_enable_unused_demo_cameras_start_1, hook_enable_unused_demo_cameras_start

PATCH_BL patch_enable_unused_demo_cameras_start_2, hook_enable_unused_demo_cameras_start

PATCH_B patch_enable_unused_demo_cameras_start_3, enableUnusedCamera_Start