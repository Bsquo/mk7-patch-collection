.include "macros.s"

.arm

PATCH_BL patch_input_viewer_create, hook_input_viewer_create
PATCH_BL patch_input_viewer_delete, hook_input_viewer_delete
