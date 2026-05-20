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

#include <nw/lyt/Pane.hpp>

namespace mod {

InputViewer *s_input_viewer = nullptr;

sead::Vector3f InputViewer::BUTTON_POSITIONS[NUM_INPUTS] = {
    {0.0f, 0.0f, 0.0f},      // BUTTON_A
    {-20.0f, -20.0f, 0.0f},  // BUTTON_B
    {-20.0f, 20.0f, 0.0f},   // BUTTON_X
    {-40.0f, 0.0f, 0.0f},    // BUTTON_Y

    {-40.0f, 60.0f, 0.0f},   // BUTTON_L
    {-6.55f, 60.0f, 0.0f},   // BUTTON_R

    {-70.0f, 0.0f, 0.0f},    // BUTTON_STICK
};

InputViewer::InputViewer(Sequence::RacePage *race_page) {
    m_inputs[BUTTON_A] = mod::utils::setupControl<mod::InputViewerButton>(race_page, "input_viewer", "input_viewer");
    m_inputs[BUTTON_A]->setButton(System::KDPad::BUTTON_A);
    m_inputs[BUTTON_A]->setType(InputViewerInput::EType::TYPE_BUTTON);

    m_inputs[BUTTON_B] = mod::utils::setupControl<mod::InputViewerButton>(race_page, "input_viewer", "input_viewer");
    m_inputs[BUTTON_B]->setButton(System::KDPad::BUTTON_B);
    m_inputs[BUTTON_B]->setType(InputViewerInput::EType::TYPE_BUTTON);

    m_inputs[BUTTON_X] = mod::utils::setupControl<mod::InputViewerButton>(race_page, "input_viewer", "input_viewer");
    m_inputs[BUTTON_X]->setButton(System::KDPad::BUTTON_X);
    m_inputs[BUTTON_X]->setType(InputViewerInput::EType::TYPE_BUTTON);

    m_inputs[BUTTON_Y] = mod::utils::setupControl<mod::InputViewerButton>(race_page, "input_viewer", "input_viewer");
    m_inputs[BUTTON_Y]->setButton(System::KDPad::BUTTON_Y);
    m_inputs[BUTTON_Y]->setType(InputViewerInput::EType::TYPE_BUTTON);

    m_inputs[BUTTON_L] = mod::utils::setupControl<mod::InputViewerButton>(race_page, "input_viewer", "input_viewer");
    m_inputs[BUTTON_L]->setButton(System::KDPad::BUTTON_L);
    m_inputs[BUTTON_L]->setType(InputViewerInput::EType::TYPE_SHOULDER);
    m_inputs[BUTTON_L]->setScale(3.0f, 0.8f);

    m_inputs[BUTTON_R] = mod::utils::setupControl<mod::InputViewerButton>(race_page, "input_viewer", "input_viewer");
    m_inputs[BUTTON_R]->setButton(System::KDPad::BUTTON_R);
    m_inputs[BUTTON_R]->setType(InputViewerInput::EType::TYPE_SHOULDER);
    m_inputs[BUTTON_R]->setScale(3.0f, 0.8f);

    m_inputs[BUTTON_STICK] = mod::utils::setupControl<mod::InputViewerButton>(race_page, "input_viewer", "input_viewer");
    m_inputs[BUTTON_STICK]->setType(InputViewerInput::EType::TYPE_STICK);
    m_inputs[BUTTON_STICK]->setScale(1.5f, 1.5f);
}

InputViewer::~InputViewer() {
    // The controls are deleted automatically by the RacePage,
    // thus we don't have to delete them ourselves manually here.
    // Same with RacePage itself.
    clear();
}

void InputViewer::clear() {
    for (u32 i = 0; i < NUM_INPUTS; i++)
        m_inputs[i] = nullptr;

    m_race_page = nullptr;
}

void InputViewer::applyInputPos() {
    UI::ControlSight::ElementHandle root_element;

    for (u32 i = 0; i < NUM_INPUTS; i++) {
        root_element.m_element = m_inputs[i]->m_control_sight->getElementHandle("R_center", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
        
        if (root_element.m_element == nullptr)
            continue;
        
        m_inputs[i]->setPos(root_element, BUTTON_POSITIONS[i]);
    }
}

////////////////////////////////////////////////////////

void InputViewerInput::AnimationDefine::defineAnimation() {
    initAnimationFamilyList(1);
    initAnimationFamily(0, "G_button_anim", 1);
    initAnimation(0, "button_anim", UI::ControlAnimator::AnimationItem::NO_PLAY);
}

void InputViewerInput::onReset() {
    release();
}

void InputViewerInput::setHoldState(bool isHold) {
    f32 ANIM_FRAME = 0.0f;

    switch (m_type) {
        case EType::TYPE_BUTTON:
        case EType::TYPE_STICK:
            ANIM_FRAME = 0.0f;
            break;

        case EType::TYPE_SHOULDER:
            ANIM_FRAME = 2.0f;
            break;

        default:
            return;
    }

    if (isHold)
        ANIM_FRAME += 1.0f;

    m_animator->getAnimationFamily(0)->setAnimation(0, ANIM_FRAME);
}

// Display the "button held" graphic (fill)
void InputViewerInput::hold() {
    setHoldState(true);
}

// Display the "button released" graphic (no fill)
void InputViewerInput::release() {
    setHoldState(false);
}

sead::Vector2f InputViewerInput::getScale() {
    nw::lyt::Pane *root_pane = getRootPane();

    if (root_pane == nullptr)
        return sead::Vector2f(0.0f, 0.0f);
    
    return sead::Vector2f(root_pane->m_scale.x, root_pane->m_scale.y);
}

void InputViewerInput::setScale(f32 x, f32 y) {
    nw::lyt::Pane *root_pane = getRootPane();

    if (root_pane != nullptr) {
        root_pane->m_scale.x = x;
        root_pane->m_scale.y = y;
    }
}

void InputViewerInput::setType(EType type) {
    m_type = type;
}

InputViewerInput::EType InputViewerInput::getType() const {
    return m_type;
}

////////////////////////////////////////////////////////

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

void InputViewerButton::setButton(u16 button) {
    m_button = button;
}

////////////////////////////////////////////////////////

void InputViewerStick::onReset() {
    hold();     // Ensure that the stick image appears filled

    m_stick.set(7, 7);
}

void InputViewerStick::onCalc() {
    s16 player_idx = RaceSys::GetRaceInfo()->m_detail_kart_id;
    System::KDPlayerPad *pad = static_cast<System::KDPlayerPad *>(Kart::GetDirector()->getKart(player_idx)->m_player_pad);
    System::KDPadAddBase::KDPadDataOnFrame *data = pad->m_pad_add_base->m_data_on_frame;

    if (pad == nullptr)
        return;

    m_stick.set(data->m_stick_x, data->m_stick_y);
}

void InputViewerStick::setButton(u16 button) {}

}

////////////////////////////////////////////////////////

HOOK void inputViewer_create() {
    Sequence::RacePage *race_page;
    READ_ARM_REG(r4, race_page);

    mod::s_input_viewer = new mod::InputViewer(race_page);
    mod::s_input_viewer->applyInputPos();
}

HOOK void inputViewer_delete() {
    if (mod::s_input_viewer != nullptr)
        delete mod::s_input_viewer;
}
