#include "mod_common.hpp"
#include "examples/input_viewer/input_viewer.hpp"
#include "examples/class_lr_select/class_lr_select.hpp"
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
    m_raw_buttons[INPUT_A] = System::KDPad::RAW_BUTTON_A;

    m_button_panes[INPUT_B][OFF] = getElement("P_button_b_off", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_button_panes[INPUT_B][ON] = getElement("P_button_b_on", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_buttons[INPUT_B] = System::KDPad::BUTTON_B;
    m_raw_buttons[INPUT_B] = System::KDPad::RAW_BUTTON_B;

    m_button_panes[INPUT_X][OFF] = getElement("P_button_x_off", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_button_panes[INPUT_X][ON] = getElement("P_button_x_on", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_buttons[INPUT_X] = System::KDPad::BUTTON_X;

    m_button_panes[INPUT_Y][OFF] = getElement("P_button_y_off", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_button_panes[INPUT_Y][ON] = getElement("P_button_y_on", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_buttons[INPUT_Y] = System::KDPad::BUTTON_Y;

    m_button_panes[INPUT_L][OFF] = getElement("P_button_l_off", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_button_panes[INPUT_L][ON] = getElement("P_button_l_on", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_buttons[INPUT_L] = System::KDPad::BUTTON_L;
    m_raw_buttons[INPUT_L] = System::KDPad::RAW_BUTTON_L;

    m_button_panes[INPUT_R][OFF] = getElement("P_button_r_off", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_button_panes[INPUT_R][ON] = getElement("P_button_r_on", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_buttons[INPUT_R] = System::KDPad::BUTTON_R;
    m_raw_buttons[INPUT_R] = System::KDPad::RAW_BUTTON_R;

    m_button_panes[INPUT_FIRST_PERSON][ON] = getElement("P_wheel", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_buttons[INPUT_FIRST_PERSON] = System::KDPad::BUTTON_FIRST_PERSON;

    // Stick
    m_stick_pane_element.m_element = m_control_sight->getElementHandle("P_button_stick", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    m_stick_pane = static_cast<nw::lyt::Pane *>(m_stick_pane_element.m_element);
    m_stick_text = static_cast<nw::lyt::TextBox *>(getElement("T_stick_values", UI::ControlSight::EElementType::ELEMENT_TYPE_TEXTBOX));

    // Background
    if (g_input_viewer_option == INPUT_VIEWER_NO_BG) {
        nw::lyt::Pane *bg_pane = getElement("P_bg", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
        m_control_sight->setVisibleImpl((u32) bg_pane, false);
    }

    setRootPos(-125.0f, -60.0f);
    setRootScale(0.7f, 0.7f);
    m_stick_original_pos.set(m_stick_pane->m_translate.x, m_stick_pane->m_translate.y);

    m_prev_buttons = -1;
}

void InputViewer::onCalc() {
    s16 player_idx = RaceSys::GetRaceInfo()->m_detail_kart_id;
    Kart::Vehicle *vehicle = Kart::GetDirector()->getKart(player_idx);
    System::KDPadAddBase::KDPadDataOnFrame *data = vehicle->m_player_pad->m_pad_add_base->m_data_on_frame;
    bool is_mirror_mode = RaceSys::GetRaceInfo()->m_is_mirror_mode;

    // CPUs use the "raw" buttons for inputs instead
    if (vehicle->m_is_ai_pad_controlled) {
        calcRaw(data, is_mirror_mode);
    }
    else {
        calcNormal(data, is_mirror_mode);
    }
}

void InputViewer::calcNormal(const System::KDPadAddBase::KDPadDataOnFrame *data, bool is_mirror_mode) {
    if (data->m_buttons != m_prev_buttons) {
        // Calc buttons
        for (u32 i = 0; i < NUM_OFF_ON_INPUTS; i++) {
            if (data->m_buttons & m_buttons[i]) {
                buttonOn(i);
            }
            else {
                buttonOff(i);
            }
        }

        m_prev_buttons = data->m_buttons;
    }

    if (data->m_stick_x != m_prev_stick_x || data->m_stick_y != m_prev_stick_y) {
        // Calc stick
        sead::Vector3f new_stick_pos(0.0f, 0.0f, 6.0f);     // We assign the position Z to 6 so that it renders in front of the outer ring.
        new_stick_pos.x = m_stick_original_pos.x + (data->m_stick_x - 7) * 2.0f;
        new_stick_pos.y = m_stick_original_pos.y + (data->m_stick_y - 7) * 2.0f;

        if (is_mirror_mode) {
            new_stick_pos.x = (new_stick_pos.x * -1.0f) + (m_stick_original_pos.x * 2.0f);
        }

        setPos(m_stick_pane_element, new_stick_pos);

        // Calc stick text
        wchar_t buffer[64];
        s8 stick_x = data->m_stick_x;

        if (is_mirror_mode) {
            stick_x = (stick_x * -1) + 14;
        }

        __2swprintf(buffer, ARRAY_COUNT(buffer), L"(%2hhd, %2hhd)", stick_x, data->m_stick_y);
        m_stick_text->SetString(buffer, 0);

        m_prev_stick_x = data->m_stick_x;
        m_prev_stick_y = data->m_stick_y;
    }
}

void InputViewer::calcRaw(const System::KDPadAddBase::KDPadDataOnFrame *data, bool is_mirror_mode) {
    if (data->m_raw_buttons != m_prev_raw_buttons) {
        // Calc buttons
        for (u32 i = 0; i < NUM_OFF_ON_INPUTS; i++) {
            if (data->m_raw_buttons & m_raw_buttons[i]) {
                buttonOn(i);
            }
            else {
                buttonOff(i);
            }
        }

        m_prev_raw_buttons = data->m_raw_buttons;
    }

    if (data->m_raw_stick.x != m_prev_raw_stick.x || data->m_raw_stick.y != m_prev_raw_stick.y) {
        // Calc stick
        sead::Vector3f new_stick_pos(0.0f, 0.0f, 6.0f);     // We assign the position Z to 6 so that it renders in front of the outer ring.
        new_stick_pos.x = m_stick_original_pos.x + data->m_raw_stick.x * 14.0f;
        new_stick_pos.y = m_stick_original_pos.y + data->m_raw_stick.y * 14.0f;

        if (is_mirror_mode) {
            new_stick_pos.x = (new_stick_pos.x * -1.0f) + (m_stick_original_pos.x * 2.0f);
        }

        setPos(m_stick_pane_element, new_stick_pos);

        // Calc stick text
        wchar_t buffer[64];
        f32 stick_x = data->m_raw_stick.x;

        if (is_mirror_mode) {
            stick_x *= -1.0f;
        }

        __2swprintf(buffer, ARRAY_COUNT(buffer), L"(%.5f, %.5f)", stick_x, data->m_raw_stick.y);
        m_stick_text->SetString(buffer, 0);

        m_prev_raw_stick.set(data->m_raw_stick.x, data->m_raw_stick.y);
    }
}

void InputViewer::setRootPos(f32 x, f32 y) {
    UI::ControlSight::ElementHandle root_pane_handle;

    root_pane_handle.m_element = m_control_sight->getElementHandle("R_center", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    nw::lyt::Pane *root_pane = static_cast<nw::lyt::Pane *>(root_pane_handle.m_element);
    setPos(root_pane_handle, sead::Vector3f(x, y, root_pane->m_translate.z));
}

void InputViewer::setRootScale(f32 x, f32 y) {
    UI::ControlSight::ElementHandle root_pane_handle;

    root_pane_handle.m_element = m_control_sight->getElementHandle("R_center", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    nw::lyt::Pane *root_pane = static_cast<nw::lyt::Pane *>(root_pane_handle.m_element);
    
    root_pane->m_scale.x = x;
    root_pane->m_scale.y = y;
}

nw::lyt::Pane *InputViewer::getElement(const sead::SafeString & name, const UI::ControlSight::EElementType element_type) {
    UI::ControlSight::ElementHandle element_handle;
    element_handle.m_element = m_control_sight->getElementHandle(name, element_type);
    
    return static_cast<nw::lyt::Pane *>(element_handle.m_element);
}

void InputViewer::buttonOff(u32 input) {
    if (input == INPUT_FIRST_PERSON) {
        m_control_sight->setVisibleImpl((u32) m_button_panes[INPUT_FIRST_PERSON][ON], false);
        return;
    }

    m_control_sight->setVisibleImpl((u32) m_button_panes[input][OFF], true);
    m_control_sight->setVisibleImpl((u32) m_button_panes[input][ON], false);
}

void InputViewer::buttonOn(u32 input) {
    if (input == INPUT_FIRST_PERSON) {
        m_control_sight->setVisibleImpl((u32) m_button_panes[INPUT_FIRST_PERSON][ON], true);
        return;
    }

    m_control_sight->setVisibleImpl((u32) m_button_panes[input][OFF], false);
    m_control_sight->setVisibleImpl((u32) m_button_panes[input][ON], true);
}

}

/////////////////////////////////////////////

HOOK void inputViewer_create() {
    Sequence::RacePage *race_page;
    READ_ARM_REG(r4, race_page);

    if (g_input_viewer_option != INPUT_VIEWER_OFF)
        mod::utils::setupBothControls<mod::InputViewer>(race_page, "input_viewer", "input_viewer");
}
