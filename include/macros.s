/* -----------------------
   Hook macro
----------------------- */
/* Saves all registers, branches to `target_func` (`bl`),
   then runs the original instructions (`original_instr`), restores all registers
   and branches back.
*/
.macro HOOK name, target_func, original_instr:vararg
    .global \name
\name:
    push {r0-r12, lr}
    bl \target_func
    pop  {r0-r12, lr}
    \original_instr
    bx lr
.endm

/* -----------------------
   Patch macro
----------------------- */
/* Adds a `bl` instruction that branches to `hook_func` */
.macro PATCH_BL name, hook_func
    .section .patch_\name
    .global patch_\name
patch_\name:
    bl \hook_func
.endm

/* Adds a `b` instruction that branches to `hook_func` */
.macro PATCH_B name, hook_func
    .section .patch_\name
    .global patch_\name
patch_\name:
    b \hook_func
.endm