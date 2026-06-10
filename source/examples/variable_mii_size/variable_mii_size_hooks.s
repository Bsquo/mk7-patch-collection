.include "macros.s"

.arm
.text

HOOK_RET_VEC3 hook_variable_mii_size_determine_vehicle_scale, variableMiiSize_determineVehicleScale, \
    bx lr

HOOK_RET_INT hook_variable_mii_size_determine_driver_size_type, variableMiiSize_determineDriverSizeType, \
    nop

HOOK hook_variable_mii_size_set_mii_voice_pitch, variableMiiSize_setMiiVoicePitch, \
    ldr r0,[r4, #0x1e0]

HOOK hook_variable_mii_size_determine_stats, variableMiiSize_determineStats, \
    cpy r4, r0
