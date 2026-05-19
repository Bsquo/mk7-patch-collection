#include "mod_common.hpp"
#include "examples/input_viewer/input_viewer.hpp"
#include "utils/ext_base_menu_page.hpp"

#include "Kart/Director.hpp"
#include "Kart/Vehicle/Vehicle.hpp"
#include "Sequence/RacePage.hpp"
#include "System/RootSystem.hpp"
#include "System/KDPadDirector.hpp"
#include "System/KDPlayerPad.hpp"
#include "UI/ControlAnimator.hpp"

#include "RaceSys/RaceInfo/Get.hpp"

namespace mod {

const f32 InputViewerButton::ANIM_RELEASE    = 0.0f;
const f32 InputViewerButton::ANIM_HOLD       = 1.0f;

void InputViewerButton::AnimationDefine::defineAnimation() {
    initAnimationFamilyList(1);
    initAnimationFamily(0, "G_button_anim", 1);
    initAnimation(0, "button_anim", UI::ControlAnimator::AnimationItem::NO_PLAY);
}

void InputViewerButton::onReset() {
    release();
}

void InputViewerButton::onCalc() {
    s16 player_idx = RaceSys::GetRaceInfo()->m_detail_kart_id;
    System::KDPlayerPad *pad = static_cast<System::KDPlayerPad *>(Kart::GetDirector()->getKart(player_idx)->m_player_pad);
    
    if (pad == nullptr)
        return;

    if (pad->m_pad_add_base->m_data_on_frame->m_buttons & m_button) {
        hold();
    }
    else {
        release();
    }
}

// Display the "button held" graphic (fill)
void InputViewerButton::hold() {
    m_animator->getAnimationFamily(0)->setAnimation(0, ANIM_HOLD);
}

// Display the "button released" graphic (no fill)
void InputViewerButton::release() {
    m_animator->getAnimationFamily(0)->setAnimation(0, ANIM_RELEASE);
}

void InputViewerButton::setParams(EType type, u16 button) {
    m_type = type;
    m_button = button;
}

////////////////////////////////////////////////////////

HOOK void inputViewer_initControl() {
    Sequence::RacePage *race_page;
    READ_ARM_REG(r4, race_page);

    mod::InputViewerButton *input_viewer_a = mod::utils::setupControl<mod::InputViewerButton>(race_page, "input_viewer", "input_viewer");
    input_viewer_a->setParams(mod::InputViewerButton::EType::BUTTON_CIRCLE, System::KDPad::BUTTON_A);
}

}