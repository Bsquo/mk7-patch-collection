#include "utils/my_menu_simple_message.hpp"

#include "UI/ControlSight.hpp"

#include <nw/lyt/TextBox.hpp>

namespace mod {
namespace utils {

void MyMenuSimpleMessage::setMessage(const UI::MessageString &message) {
    UI::ControlSight::ElementHandle textbox_handle;
    textbox_handle.m_element = m_control_sight->getElementHandle("T_text", UI::ControlSight::EElementType::ELEMENT_TYPE_TEXTBOX);
    
    if (textbox_handle.m_element == nullptr)
        return;
    
    m_control_sight->replaceMessageImpl((u32) textbox_handle.m_element, message, nullptr, nullptr);  
}

void MyMenuSimpleMessage::setTextboxSize(f32 x, f32 y) {
    UI::ControlSight::ElementHandle textbox_handle;
    textbox_handle.m_element = m_control_sight->getElementHandle("T_text", UI::ControlSight::EElementType::ELEMENT_TYPE_TEXTBOX);
    
    if (textbox_handle.m_element == nullptr)
        return;
    
    nw::lyt::Size font_size(x, y);
    nw::lyt::TextBox *textbox = static_cast<nw::lyt::TextBox *>(textbox_handle.m_element);
    textbox->mFontSize = font_size;
}

}
}