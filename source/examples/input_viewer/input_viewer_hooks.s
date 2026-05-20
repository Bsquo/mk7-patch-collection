.include "macros.s"

.arm
.text

HOOK hook_input_viewer_create, inputViewer_create, \
    ldr r0, [r4, #0x26c]
