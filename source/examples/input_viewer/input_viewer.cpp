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

#include <wchar.h>

namespace mod {

void InputViewer::onCreate(const Control::CreateArg *) {
    // Buttons
    m_button_panes[INPUT_A][OFF] = getElement("P_button_a_off", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_button_panes[INPUT_A][ON] = getElement("P_button_a_on", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_buttons[INPUT_A] = System::KDPad::BUTTON_A;

    m_button_panes[INPUT_B][OFF] = getElement("P_button_b_off", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_button_panes[INPUT_B][ON] = getElement("P_button_b_on", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_buttons[INPUT_B] = System::KDPad::BUTTON_B;

    m_button_panes[INPUT_X][OFF] = getElement("P_button_x_off", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_button_panes[INPUT_X][ON] = getElement("P_button_x_on", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_buttons[INPUT_X] = System::KDPad::BUTTON_X;

    m_button_panes[INPUT_Y][OFF] = getElement("P_button_y_off", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_button_panes[INPUT_Y][ON] = getElement("P_button_y_on", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_buttons[INPUT_Y] = System::KDPad::BUTTON_Y;

    m_button_panes[INPUT_L][OFF] = getElement("P_button_l_off", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_button_panes[INPUT_L][ON] = getElement("P_button_l_on", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_buttons[INPUT_L] = System::KDPad::BUTTON_L;

    m_button_panes[INPUT_R][OFF] = getElement("P_button_r_off", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_button_panes[INPUT_R][ON] = getElement("P_button_r_on", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_buttons[INPUT_R] = System::KDPad::BUTTON_R;

    // Stick
    m_stick_pane = getElement("P_button_stick", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_stick_text = static_cast<nw::lyt::TextBox*>(getElement("T_stick_values", UI::ControlSight::EElementType::ELEMENT_TYPE_TEXTBOX));
}

void InputViewer::onCalc() {
    s16 player_idx = RaceSys::GetRaceInfo()->m_detail_kart_id;
    System::KDPlayerPad *pad = static_cast<System::KDPlayerPad *>(Kart::GetDirector()->getKart(player_idx)->m_player_pad);
    System::KDPadAddBase::KDPadDataOnFrame *data = pad->m_pad_add_base->m_data_on_frame;

    if (pad == nullptr)
        return;

        // Calc buttons
    for (u32 i = 0; i < NUM_OFF_ON_INPUTS; i++) {
        if (data->m_buttons & m_buttons[i]) {
            buttonOn(i);
        }
        else {
            buttonOff(i);
        }
    }

    // Calc stick
    wchar_t buffer[64];
    __2swprintf(buffer, ARRAY_COUNT(buffer), L"(%hhd , %hhd)", data->m_stick_x, data->m_stick_y);
    m_stick_text->SetString(buffer, 0);
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
