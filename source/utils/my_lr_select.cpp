#include "common.hpp"
#include "utils/my_lr_select.hpp"
#include "utils/ext_base_menu_page.hpp"

#include "UI/ControlDirector.hpp"
#include "UI/ControlInitializer.hpp"
#include "UI/EKeyID.hpp"
#include "UI/UIUtil.hpp"

namespace mod {
namespace utils {

s32 MyLRSelect::num_lr_select = 0;
s32 MyLRSelect::options_array[100] = {0};

void MyLRSelect::onReset() {
    LRSelect::onReset();

    m_option = options_array[id];
    updateSelection();
}

void MyLRSelect::keyHandlerCursor(s32 param_1, s32 key) {
    // If pressing left
    bool rollOver = (key & UI::EKeyID::KEY_DPAD_LEFT) && (m_option == 0);

    LRSelect::keyHandlerCursor(param_1, key);
    
    // This makes `updateSelection` force rolling over 
    if (rollOver)
        m_option = -1;

    updateSelection();
}

void MyLRSelect::selectHandlerOn(s32 param_1, s32 param_2) {
    LRSelect::selectHandlerOn(param_1, param_2);

    if (caption != nullptr)
        caption->animIn();
}

void MyLRSelect::selectHandlerOff(s32 param_1, s32 param_2) {
    LRSelect::selectHandlerOff(param_1, param_2);

    if (caption != nullptr)
        caption->animOut();
}

MyLRSelect *MyLRSelect::createLRSelect(Sequence::BasePage *menu, bool create_bg, EDesign design) {
    MyLRSelectBg *bg = nullptr;
    const char *control_filename = nullptr;

    if (create_bg) {
        bg = mod::utils::setupControl<MyLRSelectBg>(menu, "game_setup_bg", "game_setup_bg");
    }

    switch (design) {
        case DESIGN_0:
        default:
            control_filename = "game_setup";
            break;

        case DESIGN_1:
            control_filename = "game_setup_L";
            break;

        case DESIGN_2:
            control_filename = "game_setup_LL";
            break;
    }

    // We will use "game_setup_LL" and "game_setup_2_00" as examples
    MyLRSelect *my_lr_select = mod::utils::setupControl<MyLRSelect>(menu, control_filename, "game_setup_2_00");

    if (create_bg) {
        my_lr_select->setBg(bg);
    }

    return my_lr_select;
}

void MyLRSelect::initSettings(const Settings &settings) {
    nw::lyt::TextBox *name_textbox = static_cast<nw::lyt::TextBox*>(m_elements[0].m_element);

    m_selected_option_idx = 0;
    m_settings = settings;

    // Set name string
    setVal(0, settings.m_num_options, m_settings.m_default_option);
    m_control_sight->replaceMessageImpl((u32) name_textbox, m_settings.name, nullptr, nullptr);
    UI::UIUtil::AdjustTextWidth(name_textbox, m_settings.name);

    m_control_sight->replaceMessageImpl((u32) m_val_textbox, m_settings.options_text[m_settings.m_default_option], nullptr, nullptr);
    UI::UIUtil::AdjustTextWidth(m_val_textbox, m_settings.options_text[m_settings.m_default_option]);
}

void MyLRSelect::setOnApply(OnApplyCallback callback) {
    onApply = callback;
}

void MyLRSelect::initCaption(Sequence::BasePage *menu, bool do_hide_background, const UI::MessageString &message) {
    caption = MyPrintf::createPrint(menu, do_hide_background);

    if (caption != nullptr) {
        caption->setMessage(message);
    }
}

void MyLRSelect::setPosY(f32 pos_y) {
    getRootPane()->m_translate.y = pos_y;
}

void MyLRSelect::updateSelection() {
    if (m_option >= m_num_options) {
        m_option = 0;
    }
    else if (m_option < 0) {
        m_option = m_num_options - 1;
    }

    m_control_sight->replaceMessageImpl((u32) m_val_textbox, m_settings.options_text[m_option], nullptr, nullptr);

    if (onApply) {
        onApply(this);
    }

    options_array[id] = m_option;
}

}
}