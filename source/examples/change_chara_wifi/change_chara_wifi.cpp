#include "common.hpp"
#include "UI/MessageData.hpp"
#include "Sequence/ControlSlider.hpp"
#include "Sequence/MenuWiFi_Confirm.hpp"
#include "Sound/SndSeEvent.hpp"
#include "LMS_MessageID.hpp"

UI::MenuButton *change_chara_button = nullptr;

void setButtonPosY(UI::MenuButton *button, f32 pos_y) {
    UI::ControlSight::ElementHandle pane_handle;
    // Often times `R_center` is the name of the element at the root of the button's layout
    // So we can use this to move the button
    pane_handle.m_element = button->m_control_sight->getElementHandle("R_center", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    
    if (pane_handle.m_element == nullptr)
        return;
    
    button->setPosY(pane_handle, pos_y);
}

HOOK void changeCharaWifi_initControl() {
    Sequence::MenuWifi_Confirm *menu;
    READ_ARM_REG(r4, menu);

    // First, we create the extra "Character" button
    change_chara_button = menu->setupControl<UI::MenuButton>("dialog_btn_long", "center");

    // Now, we assign an actual text message ID to it
    // We'll use the message ID 6329 (0x18B9), which is the string "Character" (LMS_MessageID::Characters)
    UI::MessageString change_character_button_text;
    UI::MessageDataList::getMessage(change_character_button_text, &change_chara_button->m_message_data_list, LMS_MessageID::Characters);
    UI::ControlSight::ElementHandle textbox_handle;
    textbox_handle.m_element = change_chara_button->m_control_sight->getElementHandle("T_diabtn", UI::ControlSight::EElementType::ELEMENT_TYPE_TEXTBOX);
    // The first argument of `replaceMessageImpl` should be a `nw::lyt::TextBox *`, but the symbol map says it's a `u32`
    change_chara_button->m_control_sight->replaceMessageImpl((u32) textbox_handle.m_element, change_character_button_text, nullptr, nullptr);    

    // Assign the "OK" sound effect when pressing the button
    change_chara_button->m_on_button_press_se = Sound::SndSeEvent::EEvent::SE_SYS_DECIDE_M_8A;

    // Append the button to the menu's "ControlSlider" (so that it appears sliding horizontally when the menu appears)
    Sequence::ControlSlider *control_slider = menu->m_control_slider_array[0];
    control_slider->setSlideH(change_chara_button);

    // We'll now adjust the Y pos of both the `OK` and the `Character` buttons
    // so that they fit nicely in the bottom screen
    setButtonPosY(menu->m_ok_button, 30.0f);
    setButtonPosY(change_chara_button, -30.0f);
}

HOOK void changeCharaWifi_onPageEnter() {
    // Make it so that we go to the character selection screen
    // after pressing the "Character" button

    // For this change to be effective, the file `MenuScene-Default.bss`
    // must be modified, as it contains the list of menus that the game will traverse to / from
    // after pressing a button.
    change_chara_button->m_return_code = 2;
}
