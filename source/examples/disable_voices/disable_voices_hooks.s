.include "macros.s"

.arm
.text

# From `class_lr_select.hpp`
.extern g_disable_voices

.global hook_disable_voices_1
hook_disable_voices_1:
    ldr     r12, =g_disable_voices
    ldr     r12, [r12]               @ r12 = g_disable_voices

    cmp     r12, #0
    bne     end_1                   @ if (g_disable_voices)
    
    b _ZN5Sound10SndSeEvent18playReadyToGoVoiceEl     @ Original instruction
    
    end_1:
        bx lr

.global hook_disable_voices_2
hook_disable_voices_2:
    ldr     r5, =g_disable_voices
    ldr     r5, [r5]                @ r5 = g_disable_voices

    cmp     r5, #0
    bne     end_2                   @ if (g_disable_voices)
    
    b _ZN5Sound10SndSeEvent20playCharaDecideVoiceEl     @ Original instruction
    
    end_2:
        bx lr

.global hook_disable_voices_3
hook_disable_voices_3:
    ldr     r5, =g_disable_voices
    ldr     r5, [r5]                @ r5 = g_disable_voices

    cmp     r5, #0
    bne     end_3                   @ if (g_disable_voices)
    
    b _ZN5Sound10SndSeEvent21playCharaPresentVoiceEl     @ Original instruction
    
    end_3:
        bx lr
