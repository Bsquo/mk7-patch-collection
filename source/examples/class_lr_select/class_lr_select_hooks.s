.include "macros.s"

.arm
.text

HOOK hook_class_lr_select_initControl, classLRSelect_initControl, \
    cpy r4, r0
