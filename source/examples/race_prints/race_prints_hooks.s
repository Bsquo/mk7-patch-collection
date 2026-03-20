.arm
.text

.global hook_race_prints_init
hook_race_prints_init:
    push {r0-r12, lr}
    bl racePrints_init
    pop  {r0-r12, lr}
    cpy r4, r0        @ Original instruction
    bx lr

.global hook_race_prints_calc
hook_race_prints_calc:
    push {r0-r12, lr}
    bl racePrints_calc
    pop  {r0-r12, lr}
    cpy r4, r0        @ Original instruction
    bx lr
