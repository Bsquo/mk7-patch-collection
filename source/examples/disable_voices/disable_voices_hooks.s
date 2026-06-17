.include "macros.s"

.arm
.text

# From `class_lr_select.hpp`
.extern g_disable_voices

.global hook_disable_voices_1
hook_disable_voices_1:
    ldr     r5, =g_disable_voices
    ldr     r5, [r5]                @ r5 = g_disable_voices

    cmp     r5, #0
    bne     end                     @ if (g_disable_voices)
    
    b _ZN5Sound10SndSeEvent18playReadyToGoVoiceEl     @ Original instruction
    
    end:
        bx lr

.global hook_disable_voices_2
hook_disable_voices_2:
    ldr     r5, =g_disable_voices
    ldr     r5, [r5]                @ r5 = g_disable_voices

    cmp     r5, #0
    bne     end                     @ if (g_disable_voices)
    
    b _ZN5Sound10SndSeEvent20playCharaDecideVoiceEl     @ Original instruction
    
    end:
        bx lr

.global hook_disable_voices_3
hook_disable_voices_3:
    ldr     r5, =g_disable_voices
    ldr     r5, [r5]                @ r5 = g_disable_voices

    cmp     r5, #0
    bne     end                     @ if (g_disable_voices)
    
    b _ZN5Sound10SndSeEvent21playCharaPresentVoiceEl     @ Original instruction
    
    end:
        bx lr

.global hook_disable_voices_4
hook_disable_voices_4:
    ldr     r5, =g_disable_voices
    ldr     r5, [r5]                @ r5 = g_disable_voices

    cmp     r5, #0
    bne     disable_voice           @ if (g_disable_voices)

    mov r1, #0x00
    b       end
    
    disable_voice:
        mov r1, #0x01

    end:
        strb r1, [r0, #0x25]         @ Original instruction
        bx lr
