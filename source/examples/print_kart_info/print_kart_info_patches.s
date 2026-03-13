.arm

.section .patch_print_kart_info
.global patch_print_kart_info
patch_print_kart_info:
    bl hook_print_kart_info
