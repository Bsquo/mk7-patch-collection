.arm

.section .patch_show_timer_in_gp
.global patch_show_timer_in_gp
patch_show_timer_in_gp:
    bl hook_show_timer_in_gp
