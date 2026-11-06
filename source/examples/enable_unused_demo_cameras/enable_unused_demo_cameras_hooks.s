.include "macros.s"

.arm
.text

HOOK hook_enable_unused_demo_cameras_goal, enableUnusedCamera_Goal, \
    mov r5, #0x0

HOOK hook_enable_unused_demo_cameras_start, enableUnusedCamera_Start, \
    cpy r5, r0
