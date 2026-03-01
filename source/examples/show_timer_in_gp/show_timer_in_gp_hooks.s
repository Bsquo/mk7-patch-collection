.arm
.text

.global hook_show_timer_in_gp
hook_show_timer_in_gp:
    push {r0-r12, lr}
    bl initTimeInGP
    pop  {r0-r12, lr}
    cpy r4,r0        @ Original instruction
    bx lr
