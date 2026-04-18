#include "common.hpp"
#include "Sequence/BaseRacePage.hpp"
#include "UI/TimeControl.hpp"

HOOK void initTimeInGP(Sequence::BaseRacePage* baseRacePage) {
    // baseRacePage is already in r0 when the game reaches this function,
    // therefore we don't have to move registers here
    baseRacePage->initTime();
}

HOOK void setTimeControlPos(UI::TimeControl *time_control) {
    nw::lyt::Pane *root_pane = time_control->getRootPane();
    
    root_pane->m_translate.x = -275.0f;
    root_pane->m_translate.y = -205.0f;
}
