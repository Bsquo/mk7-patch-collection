.arm

.section .patch_loader
.global loader_patch
loader_patch:
    b hook_into_loader
