.arm
.text

.global hook_print_kart_info
hook_print_kart_info:
    push {r0-r12, lr}
    bl printKartInfo
    pop  {r0-r12, lr}
    vstr.32 s0,[r4,#0x1c4]        @ Original instruction
    bx lr
