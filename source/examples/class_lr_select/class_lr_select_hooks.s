.include "macros.s"

.arm
.text

HOOK hook_class_lr_select_initControl, classLRSelect_initControl, \
    add r1, r4, #0x298
