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

/* Same as above, except this will modify the value of r0,
   so that we can execute hooks that return a number and use those
   in the original game code.
*/
.macro HOOK_RET_INT name, target_func, original_instr:vararg
    .global \name
\name:
    push {r1-r12, lr}
    bl \target_func
    pop  {r1-r12, lr}
    \original_instr
    bx lr
.endm

/* -----------------------
   Patch macro
----------------------- */
/* Adds a `bl` instruction that branches to `hook_func` */
.macro PATCH_BL name, hook_func
    .section .\name
    .global \name
\name:
    bl \hook_func
.endm

/* Adds a `b` instruction that branches to `hook_func` */
.macro PATCH_B name, hook_func
    .section .\name
    .global \name
\name:
    b \hook_func
.endm

.macro PATCH_WORD name, values:vararg
    .section .\name
    .global \name
\name:
    .word \values
.endm

.macro PATCH_HALF name, values:vararg
    .section .\name
    .global \name
\name:
    .hword \values
.endm

.macro PATCH_BYTES name, values:vararg
    .section .\name
    .global \name
\name:
    .byte \values
.endm
