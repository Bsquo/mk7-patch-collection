#include "common.hpp"
#include "utils/my_lr_select.hpp"
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

MyLRSelect *MyLRSelect::createLRSelect(Sequence::BasePage *menu, bool create_bg) {
    UI::ControlInitializer *initializer = menu->m_control_initializer;
    UI::ControlDirector *director = initializer->m_control_director;
    MyLRSelectBg *bg = nullptr;

    if (create_bg) {
        MyLRSelectBg::CreateArg bg_create_arg;
        // TODO: Hacky way to emit UI::LRSelectBg::AnimationDefine's vtable
        new (&bg_create_arg.m_animation_define) MyLRSelectBg::AnimationDefine;

        bg = new MyLRSelectBg();
        bg->initNode(director);
        director->appendNode(bg);

        initializer->m_control = bg;
        initializer->initCreateArg(&bg_create_arg, "game_setup_bg", "game_setup_bg");
        initializer->m_control_create_arg = &bg_create_arg;
        initializer->endSetupControl(&bg_create_arg);

        menu->m_controls.pushBack(bg);
    }

    CreateArg create_arg;
    // TODO: Hacky way to emit UI::LRSelect::AnimationDefine's vtable
    new (&create_arg.m_animation_define) MyLRSelect::AnimationDefine;

    MyLRSelect *my_lr_select = new MyLRSelect();
    my_lr_select->initNode(director);
    director->appendNode(my_lr_select);

    initializer->m_control = my_lr_select;
    // We will use "game_setup" and "game_setup_5_00" as examples
    initializer->initCreateArg(&create_arg, "game_setup", "game_setup_5_00");
    initializer->m_control_create_arg = &create_arg;
    initializer->endSetupControl(&create_arg);

    menu->m_controls.pushBack(my_lr_select);

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