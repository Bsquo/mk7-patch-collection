.include "macros.s"

.arm

# Save the goal camera ptr into DemoCameraDirector's `m_current_demo_camera` field (instead of nullptr)
PATCH_ASM patch_enable_unused_demo_cameras_goal_1, \
    str r0,[r4, #0x5c]

# Ensure that the goal camera's player idx is 0 instead of -1
# TODO: Should be saving `m_camera_target_player_id` instead of forcing it to always be 0.
#       The player idx is automatically set to `m_camera_target_player_id` in function `Demo::DemoCameraFollowBase::initInner`.
#       Find out why it doesn't do this for the goal camera.
PATCH_ASM patch_enable_unused_demo_cameras_goal_2, \
    mov r8, #0x0

PATCH_BL patch_enable_unused_demo_cameras_goal_3, hook_enable_unused_demo_cameras_goal

PATCH_BL patch_enable_unused_demo_cameras_start_1, hook_enable_unused_demo_cameras_start

PATCH_B patch_enable_unused_demo_cameras_start_2, enableUnusedCamera_Start