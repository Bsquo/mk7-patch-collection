#include "mod_common.hpp"
#include "examples/input_viewer/input_viewer.hpp"
#include "utils/ext_base_menu_page.hpp"

#include "Sequence/RacePage.hpp"
#include "System/RootSystem.hpp"
#include "System/KDPadDirector.hpp"
#include "System/KDPlayerPad.hpp"
#include "System/KDPadControllerCore.hpp"
#include "UI/ControlAnimator.hpp"

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
    s32 index = static_cast<s32>(System::KDPadDirector::eKDPadListIndex::PLAYER);
    System::KDPlayerPad *pad = static_cast<System::KDPlayerPad *>(System::g_root_system->get_pad_director()->m_pads[index]);
    
    if (pad == nullptr)
        return;

    if (pad->m_pad_controller->m_pad_controller_core->mPadHold & m_button) {
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

void InputViewerButton::setParams(EType type, sead::CtrController::ButtonEnum button) {
    m_type = type;
    m_button = button;
}

////////////////////////////////////////////////////////

HOOK void inputViewer_initControl() {
    Sequence::RacePage *race_page;
    READ_ARM_REG(r4, race_page);

    mod::InputViewerButton *input_viewer_a = mod::utils::setupControl<mod::InputViewerButton>(race_page, "input_viewer", "input_viewer");
    input_viewer_a->setParams(mod::InputViewerButton::EType::BUTTON_CIRCLE, sead::CtrController::ButtonEnum::BUTTON_A);
}

}