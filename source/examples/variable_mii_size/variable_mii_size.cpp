#include "mod_common.hpp"
#include "examples/class_lr_select/class_lr_select.hpp"

#include "Kart/KartConstructInfoAccessor.hpp"
#include "Kart/KartConstructInfoData.hpp"
#include "Kart/RawKartConstructInfoBin.hpp"
#include "Kart/Vehicle/Vehicle.hpp"
#include "Kart/PartsDriveParam.hpp"
#include "Mii/MiiEngine.hpp"
#include "RaceSys/EDriverID.hpp"
#include "Sound/SndActorKart.hpp"
#include "Sound/SndEngine.hpp"
#include "Sound/SndHandle.hpp"
#include "Sound/SoundID.hpp"
#include "Sound/SndSeEvent.hpp"
#include "System/PlayerData.hpp"
#include "System/RootSystem.hpp"
#include "System/SystemEngine.hpp"

#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace mod
{
    Kart::KartConstructInfoAccessor::EDriverSizeType getMyMiiWeightClass() {
        System::PlayerData player_data;

        System::g_root_system->m_root_scene->get_system_engine()->getMyPlayerData(&player_data, false);
        u8 height = player_data.m_store_data.height;
        u8 weight = player_data.m_store_data.weight;

        if ((150 - height) < weight) {
            return Kart::KartConstructInfoAccessor::EDriverSizeType::LARGE;
        }
        else if ((104 - height) < weight) {
            return Kart::KartConstructInfoAccessor::EDriverSizeType::MEDIUM;
        }
        else {
            return Kart::KartConstructInfoAccessor::EDriverSizeType::SMALL;
        }
    }

    bool isMyMiiFemale() {
        System::PlayerData player_data;
        CFLAdditionalInfo additional_info;

        System::g_root_system->m_root_scene->get_system_engine()->getMyPlayerData(&player_data, false);
        System::g_root_system->m_root_scene->get_mii_engine()->getAdditionalInfo(&additional_info, player_data.m_store_data, false);

        return additional_info.skin_color_or_gender & 1;
    }

    bool isMii(RaceSys::EDriverID driver) {
        return driver == RaceSys::EDriverID::MiiMale || driver == RaceSys::EDriverID::MiiFemale;
    }

    f32 getMiiVoicePitch() {
        switch (getMyMiiWeightClass()) {
        case Kart::KartConstructInfoAccessor::EDriverSizeType::SMALL:
            return 1.1f;

        case Kart::KartConstructInfoAccessor::EDriverSizeType::MEDIUM:
            return 1.0f;

        case Kart::KartConstructInfoAccessor::EDriverSizeType::LARGE:
            return 0.9f;
        }

        return 1.0f;
    }

}

HOOK sead::Vector3f variableMiiSize_determineVehicleScale(Kart::KartConstructInfoAccessor *accessor) {
    RaceSys::EDriverID driver = accessor->m_kart_parts_construct_param.m_driver_id;

    if (g_variable_mii_size && mod::isMii(driver)) {
        switch (mod::getMyMiiWeightClass()) {
            case Kart::KartConstructInfoAccessor::EDriverSizeType::LARGE:
                return sead::Vector3f(1.10f, 1.10f, 1.10f);

            case Kart::KartConstructInfoAccessor::EDriverSizeType::MEDIUM:
                break;

            case Kart::KartConstructInfoAccessor::EDriverSizeType::SMALL:
                return sead::Vector3f(0.85f, 0.85f, 0.85f);
        }
    }

    // Original logic
    f32 body_scale = accessor->m_data->m_excel_or_manager.m_raw_file->m_character_info[static_cast<u32>(accessor->m_kart_parts_construct_param.m_driver_id)].m_inverse_scale;
    return sead::Vector3f(body_scale, body_scale, body_scale);
}

HOOK void variableMiiSize_determineDriverSizeType(Kart::KartConstructInfoAccessor::EDriverSizeType *out, Kart::KartConstructInfoAccessor *accessor) {
    RaceSys::EDriverID driver = accessor->m_kart_parts_construct_param.m_driver_id;

    if (g_variable_mii_size && mod::isMii(driver)) {
        *out = mod::getMyMiiWeightClass();
        return;
    }
    else {
        // Original function
        // TODO: Improve this
        Kart::KartConstructInfoAccessor::EDriverSizeType ret = accessor->getDriverSizeType();
        // When reaching this point, `ret` will be in r1, but not in r0.
        // Ensure that `ret` ends up in r0
        READ_ARM_REG(r1, ret);
        *out = ret;
    }
}

HOOK void variableMiiSize_setMiiVoicePitch() {
    Sound::SndActorKart *snd_actor_kart;
    READ_ARM_REG(r4, snd_actor_kart);

    RaceSys::EDriverID driver = snd_actor_kart->m_vehicle->m_driver_id;
    if (!g_variable_mii_size || !mod::isMii(driver))
        return;

    snd_actor_kart->m_driver_voice_snd_handle.m_sound_handle.setPitch(mod::getMiiVoicePitch());
}

HOOK void variableMiiSize_determineStats(void *, s32 *driver) {
    if (!g_variable_mii_size || !mod::isMii(*reinterpret_cast<RaceSys::EDriverID *>(driver)))
        return;
    
    switch (mod::getMyMiiWeightClass()) {
        case Kart::KartConstructInfoAccessor::EDriverSizeType::LARGE:
            *driver = static_cast<s32>(RaceSys::EDriverID::Bowser);
            break;

        case Kart::KartConstructInfoAccessor::EDriverSizeType::MEDIUM:
            *driver = static_cast<s32>(RaceSys::EDriverID::Mario);
            break;

        case Kart::KartConstructInfoAccessor::EDriverSizeType::SMALL:
            *driver = static_cast<s32>(RaceSys::EDriverID::Toad);
            break;
    }
}

HOOK sead::SafeString variableMiiSize_determineModel(Kart::KartConstructInfoAccessor *accessor, s32 *driverId) {
    if (g_variable_mii_size && mod::isMii(*reinterpret_cast<RaceSys::EDriverID *>(driverId))) {
        if (!mod::isMyMiiFemale()) {
            switch (mod::getMyMiiWeightClass()) {
                case Kart::KartConstructInfoAccessor::EDriverSizeType::LARGE:
                    return "lim";

                case Kart::KartConstructInfoAccessor::EDriverSizeType::MEDIUM:
                    break;

                case Kart::KartConstructInfoAccessor::EDriverSizeType::SMALL:
                    return "sim";
            }
        }
        else {
            switch (mod::getMyMiiWeightClass()) {
                case Kart::KartConstructInfoAccessor::EDriverSizeType::LARGE:
                    return "lif";

                case Kart::KartConstructInfoAccessor::EDriverSizeType::MEDIUM:
                    break;

                case Kart::KartConstructInfoAccessor::EDriverSizeType::SMALL:
                    return "sif";
            }
        }
    }

    // Original logic
    return accessor->m_data->m_excel_or_manager.m_raw_file->m_character_info[*driverId].m_short_name;
}
