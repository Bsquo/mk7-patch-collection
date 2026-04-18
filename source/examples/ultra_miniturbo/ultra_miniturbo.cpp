#include "mod_common.hpp"
#include "examples/class_lr_select/class_lr_select.hpp"

#include "Effect/KartEffect.hpp"
#include "Kart/InfoProxy.hpp"
#include "Sound/SndActorKart.hpp"
#include "Sound/SoundID.hpp"
#include <gfx/seadColor.h>

bool ultra_miniturbo_enabled[KART_MAX] = {false};

HOOK void ultraMiniturbo_init() {
    if (g_ultra_miniturbo_enabled) {
        for (u32 i = 0; i < ARRAY_COUNT(ultra_miniturbo_enabled); i++)
            ultra_miniturbo_enabled[i] = false;
    }
}

HOOK void ultraMiniturbo_calc(Effect::KartEffect *kart_effect) {
    if (g_ultra_miniturbo_enabled) {
        Kart::Vehicle *vehicle = kart_effect->m_info_proxy->m_vehicle;
        Sound::SndActorKart *snd_actor_kart = vehicle->m_snd_actor_kart;

        if (vehicle->m_miniturbo_charge >= 700.0f) {
            for (u32 i = 11; i <= 19; i++) {
                kart_effect->m_particles[i]->setColor(179.0f, 0.0f, 217.0f);
            }
            ultra_miniturbo_enabled[vehicle->m_player_id] = true;
            // TODO: Find a way to make the sound only play once.
            //       I tried playing it for one frame only but it didn't seem to work?
            snd_actor_kart->holdSound(Sound::SE_KT_DRIFT_HIBANA_BLUE, &snd_actor_kart->m_0x1c4);
        }
        else {
            // TODO: Attempt to restore orange miniturbo's original color
            ultra_miniturbo_enabled[vehicle->m_player_id] = false;
        }
    }
}

HOOK void ultraMiniturbo_updateDashParam() {
    if (g_ultra_miniturbo_enabled) {
        Kart::Vehicle *vehicle;
        READ_ARM_REG(r5, vehicle);

        if (ultra_miniturbo_enabled[vehicle->m_player_id]) {
            const f32 boost_speed = vehicle->m_current_max_speed * 1.14f;
            const f32 boost_accel = 3.0f;
            const f32 boost_frames = vehicle->m_Dash_FrmMiniT_Lv2 + 70.0f;

            vehicle->m_boost_speed = boost_speed;
            vehicle->m_boost_accel = boost_accel;
            vehicle->m_boost_frames = boost_frames;
        }
    }
}
