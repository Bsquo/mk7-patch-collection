#include "common.hpp"
#include "utils/my_printf.hpp"

#include "Sequence/BaseRacePage.hpp"
#include "Kart/Director.hpp"
#include "Kart/Vehicle/Vehicle.hpp"
#include "RaceSys/LapRankChecker.hpp"
#include "RaceSys/RaceInfo/Get.hpp"
#include <nw/lyt/TextBox.hpp>

mod::utils::MyPrintf *prints = nullptr;

HOOK void racePrints_init(Sequence::BaseRacePage *race_page) {
    // Avoid printing in Battle mode, since this example print is only meant for
    // races in mind.
    RaceSys::CRaceMode::RaceType match_type = RaceSys::GetRaceInfo()->m_race_mode.m_type;
    if (match_type == RaceSys::CRaceMode::RaceType::Balloon ||
        match_type == RaceSys::CRaceMode::RaceType::Coin) {
        return;
    }

    prints = mod::utils::MyPrintf::createPrint(race_page);
    prints->setColor(nw::ut::Color8(255, 250, 200, 255), nw::lyt::TEXTCOLOR_TOP);
    prints->setColor(nw::ut::Color8(255, 250, 0, 255),   nw::lyt::TEXTCOLOR_BOTTOM);
    prints->setLineSpace(-3.0f);
    prints->setTextAlignment(nw::lyt::ALIGN_TOP_LEFT);
}

HOOK void racePrints_calc() {
    // Avoid printing in Battle mode, since this example print is only meant for
    // races in mind.
    RaceSys::CRaceMode::RaceType match_type = RaceSys::GetRaceInfo()->m_race_mode.m_type;
    if (match_type == RaceSys::CRaceMode::RaceType::Balloon ||
        match_type == RaceSys::CRaceMode::RaceType::Coin) {
        return;
    }

    const s32 playerIdx = 0;

    Kart::Vehicle *vehicle = Kart::GetDirector()->getKart(playerIdx);
    prints->printf
    (
        0.0f, 0.0f,
        L"Frame: %d\n"
        L"Pos: (%.3f, %.3f, %.3f)\n"
        L"MT: (%03.0f/%03.0f)\n"
        L"Boost: %d\n"
        L"CP: %d\n"
        L"CKP: %d\n"
        L"Race%%: %.3f\n"
        L"MaxRace%%: %.3f\n"
        L"Speed: %.3f",
        *vehicle->m_frame,
        vehicle->m_position->x, vehicle->m_position->y, vehicle->m_position->z,
        vehicle->m_miniturbo_charge, (vehicle->m_miniturbo_charge >= 220.0f) ? 460.0f : 220.0f,
        vehicle->m_boost_frames,
        vehicle->m_lap_rank_checker->m_kart_infos[playerIdx].m_checkpoint_index,
        vehicle->m_lap_rank_checker->m_kart_infos[playerIdx].m_key_checkpoint_id,
        vehicle->m_lap_rank_checker->m_kart_infos[playerIdx].m_current_race_progress,
        vehicle->m_lap_rank_checker->m_kart_infos[playerIdx].m_max_race_progress,
        vehicle->m_forward_speed
    );
}
