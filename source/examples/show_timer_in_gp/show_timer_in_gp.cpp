#include "mod_common.hpp"
#include "RaceSys/RaceInfo/Get.hpp"
#include "Sequence/BaseRacePage.hpp"
#include "UI/TimeControl.hpp"

HOOK void initTimeInGP(Sequence::BaseRacePage* baseRacePage) {
    // baseRacePage is already in r0 when the game reaches this function,
    // therefore we don't have to move registers here
    baseRacePage->initTime();
}

HOOK void setTimeControlPos(UI::TimeControl *time_control) {
    nw::lyt::Pane *root_pane = time_control->getRootPane();
    
    RaceSys::ERaceRuleMode rule_mode = RaceSys::GetRaceInfo()->m_race_mode.m_rule_mode;
    if (rule_mode == RaceSys::ERaceRuleMode::GrandPrix ||
        rule_mode == RaceSys::ERaceRuleMode::Versus) {
        root_pane->m_translate.x = -275.0f;
        root_pane->m_translate.y = -205.0f;
    }
}
