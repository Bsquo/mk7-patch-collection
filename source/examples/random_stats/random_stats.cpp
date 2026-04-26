#include "mod_common.hpp"
#include "examples/class_lr_select/class_lr_select.hpp"

#include "Sequence/MenuData.hpp"

// original_stats_value will be in r0 when we enter this function
HOOK s32 randomStats_getRandomPointS32(s32 original_stats_value) {
    if (!g_random_stats_enabled) {
        // Original logic
        return (original_stats_value > 20) ? 20 : original_stats_value;
    }
    else {
        return Sequence::GetRandom()->m_random.getS32Range(1, 21);
    }
}
