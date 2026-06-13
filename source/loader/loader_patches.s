.arm

.section .patch_loader
.global loader_patch
loader_patch:
    b hook_into_loader

.section .patch_call_global_constructors
.global patch_call_global_constructors
patch_call_global_constructors:
    b hook_call_global_constructors
