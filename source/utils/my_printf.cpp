#include "utils/my_printf.hpp"
#include <wchar.h>

namespace mod {
namespace utils {

MyPrintf *MyPrintf::createPrint(Sequence::BasePage *menu) {
    UI::ControlInitializer *initializer = menu->m_control_initializer;
    UI::ControlDirector *director = initializer->m_control_director;
    menu->m_set_item_to_all_manipulators = true;

    CreateArg create_arg;
    // TODO: Hacky way to emit UI::MenuCaption::AnimationDefine's vtable
    new (&create_arg.m_animation_define) MyPrintf::AnimationDefine;

    MyPrintf *my_printf = new MyPrintf();
    my_printf->initNode(director);
    director->appendNode(my_printf);

    initializer->m_control = my_printf;
    initializer->initCreateArg(&create_arg, "caption", "caption");
    initializer->m_control_create_arg = &create_arg;
    initializer->endSetupControl(&create_arg);

    menu->m_set_item_to_all_manipulators = false;

    menu->m_controls_outside_manipulator_array.pushBack(my_printf);

    my_printf->reallocateStringBuffer();
    my_printf->hideBackground();

    return my_printf;
}

void MyPrintf::setMessage(const UI::MessageString &message) {
    // The first argument of `replaceMessageImpl` should be a `nw::lyt::TextBox *`, but the symbol map says it's a `u32`
    m_control_sight->replaceMessageImpl((u32) m_elements[0], message, nullptr, nullptr);  
}

void MyPrintf::setMessage(u32 message_id) {
    UI::MessageString message;

    UI::MessageDataList::getMessage(message, &m_message_data_list, message_id);
    setMessage(message);
}

void MyPrintf::setColor(const nw::ut::Color8 color, s32 color_location = nw::lyt::TEXTCOLOR_MAX) {
    nw::lyt::TextBox *textbox = getTextBox();

    if (color_location == nw::lyt::TEXTCOLOR_MAX) {
        for (s32 i = 0; i < nw::lyt::TEXTCOLOR_MAX; i++) {
            textbox->mTextColors[i] = color;
        }

        return;
    }
    
    textbox->mTextColors[color_location] = color;
}

void MyPrintf::setPos(f32 x, f32 y) {
    UI::ControlSight::ElementHandle textbox_handle;
    textbox_handle.m_element = m_control_sight->getElementHandle("T_caption_00", UI::ControlSight::EElementType::ELEMENT_TYPE_TEXTBOX);
    
    if (textbox_handle.m_element == nullptr)
        return;
    
    // We substract by -400 here to counter the +400 offset we applied to the background in `hideBackground`
    x -= 400.0f;

    setPosX(textbox_handle, x);
    setPosY(textbox_handle, y);
}

void MyPrintf::setOutlineColor(u8 color) {
    nw::lyt::TextBox *textbox = getTextBox();

    if (color >= nw::lyt::TEXT_OUTLINE_MAX)
        return;
    
    textbox->mOutlineColor = color;
}

void MyPrintf::setSize(f32 x, f32 y) {
    nw::lyt::TextBox *textbox = getTextBox();
    nw::lyt::Size font_size(x, y);
    
    textbox->mFontSize = font_size;
}

void MyPrintf::setLineSpace(f32 line_space) {
    nw::lyt::TextBox *textbox = getTextBox();
    
    textbox->mLineSpace = line_space;
}

void MyPrintf::setCharSpace(f32 char_space) {
    nw::lyt::TextBox *textbox = getTextBox();
    
    textbox->mCharSpace = char_space;
}

// Sets the text alignment position (right, center, left, etc)
void MyPrintf::setTextAlignment(u8 position) {
    nw::lyt::TextBox *textbox = getTextBox();
    
    textbox->mTextPosition = position;
}

// For this to take effect, call this function right after creating MyPrintf in your initialization function
void MyPrintf::setDrawBottomScreen(bool draw_in_bottom_screen) {
    if (draw_in_bottom_screen) {
        m_draw_screen_flag |= DRAW_ON_BOTTOM_SCREEN;
    }
    else {
        m_draw_screen_flag |= DRAW_ON_TOP_SCREEN;
    }
}

void MyPrintf::printf(f32 x, f32 y, const wchar_t *fmt, ...) {
    animKeep();
    setPos(x, y);

    va_list args;
    va_start(args, fmt);
    UI::MessageString message = formatMessage(m_wbuf, BUFFER_CHARS, fmt, args);
    va_end(args);

    setMessage(message);
}

// The intention of this function is to hide the black background. `setPos` will move the actual text
void MyPrintf::hideBackground() {
    UI::ControlSight::ElementHandle pane_handle;
    pane_handle.m_element = m_control_sight->getElementHandle("R_center", UI::ControlSight::EElementType::ELEMENT_TYPE_PANE);
    
    if (pane_handle.m_element == nullptr)
        return;

    setPosX(pane_handle, 400.0f);
}

void MyPrintf::reallocateStringBuffer() {
    nw::lyt::TextBox *textbox = getTextBox();

    textbox->FreeStringBuffer();
    // This is how the alloc string buffer size is normally obtained in `nw::lyt::TextBox::TextBox`
    textbox->AllocStringBuffer((BUFFER_CHARS >> 1) - 1);
}

UI::MessageString MyPrintf::formatMessage(wchar_t *dest_buf, u16 dest_chars, const wchar_t *fmt, va_list args) {
    if (dest_buf == nullptr || dest_chars == 0) {
        return UI::MessageString(nullptr);
    }

    s32 written = __c89vswprintf(dest_buf, dest_chars, fmt, args);
    if (written < 0) {
        dest_buf[0] = L'\0';
    }
    else if (static_cast<u16>(written) >= dest_chars) {
        dest_buf[dest_chars - 1] = L'\0';
    }

    return UI::MessageString(reinterpret_cast<char16*>(dest_buf));
}

nw::lyt::TextBox *MyPrintf::getTextBox() const {
    UI::ControlSight::ElementHandle textbox_handle;
    textbox_handle.m_element = m_control_sight->getElementHandle("T_caption_00", UI::ControlSight::EElementType::ELEMENT_TYPE_TEXTBOX);
    
    if (textbox_handle.m_element == nullptr)
        return nullptr;
    
    return static_cast<nw::lyt::TextBox*>(textbox_handle.m_element);
}
    
}
}