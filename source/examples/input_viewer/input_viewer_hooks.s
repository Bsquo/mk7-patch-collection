.include "macros.s"

.arm
.text

HOOK hook_input_viewer_initControl, inputViewer_initControl, \
    ldr r0, [r4, #0x26c]
