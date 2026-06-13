.arm
.text

.section .loader
.global hook_into_loader
hook_into_loader:
    push {r0-r12, lr}
    bl loader_main
    pop {r0-r12, lr}
    
    @ Original instruction
    bl nninitRegion
    @ Continue normal execution of `__ctr_start`
    b  0x100004

.section .loader
.global hook_call_global_constructors
hook_call_global_constructors:
    push {r0-r12, lr}
    bl __libc_init_array
    pop {r0-r12, lr}
    
    @ Original instruction
    bl nninitSetup
    @ Continue normal execution of `__ctr_start`
    b 0x10001C

