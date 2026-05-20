#include "mod_common.hpp"
#include "examples/input_viewer/input_viewer.hpp"
#include "utils/ext_base_menu_page.hpp"

#include "Kart/Director.hpp"
#include "Kart/Vehicle/Vehicle.hpp"
#include "Sequence/RacePage.hpp"
#include "System/RootSystem.hpp"
#include "System/KDPadDirector.hpp"
#include "System/KDPlayerPad.hpp"
#include "RaceSys/RaceInfo/Get.hpp"

namespace mod {

void InputViewer::onCreate(const Control::CreateArg *) {
    m_button_panes[INPUT_A][OFF] = getElement("P_button_a_off", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_button_panes[INPUT_A][ON] = getElement("P_button_a_on", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_buttons[INPUT_A] = System::KDPad::BUTTON_A;
}

void InputViewer::onReset() {
    buttonOff(INPUT_A);
}

void InputViewer::onCalc() {
    s16 player_idx = RaceSys::GetRaceInfo()->m_detail_kart_id;
    System::KDPlayerPad *pad = static_cast<System::KDPlayerPad *>(Kart::GetDirector()->getKart(player_idx)->m_player_pad);
    
    if (pad == nullptr)
        return;

    if (pad->m_pad_add_base->m_data_on_frame->m_buttons & m_buttons[INPUT_A]) {
        buttonOn(INPUT_A);
    }
    else {
        buttonOff(INPUT_A);
    }
}

nw::lyt::Pane *InputViewer::getElement(const sead::SafeString & name, const UI::ControlSight::EElementType element_type) {
    UI::ControlSight::ElementHandle element_handle;
    element_handle.m_element = m_control_sight->getElementHandle(name, element_type);
    
    if (element_handle.m_element == nullptr)
        return nullptr;
    
    return static_cast<nw::lyt::Pane *>(element_handle.m_element);
}

void InputViewer::buttonOff(u32 input) {
    m_control_sight->setVisibleImpl((u32) m_button_panes[input][OFF], true);
    m_control_sight->setVisibleImpl((u32) m_button_panes[input][ON], false);
}

void InputViewer::buttonOn(u32 input) {
    m_control_sight->setVisibleImpl((u32) m_button_panes[input][OFF], false);
    m_control_sight->setVisibleImpl((u32) m_button_panes[input][ON], true);
}

}

/////////////////////////////////////////////

HOOK void inputViewer_create() {
    Sequence::RacePage *race_page;
    READ_ARM_REG(r4, race_page);

    mod::utils::setupControl<mod::InputViewer>(race_page, "input_viewer", "input_viewer");
}
