.include "macros.s"

.arm

PATCH_BL patch_show_timer_in_gp, hook_show_timer_in_gp
PATCH_BL patch_set_time_control_pos, hook_set_time_control_pos
