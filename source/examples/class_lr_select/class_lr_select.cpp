#include "examples/class_lr_select/class_lr_select.hpp"
#include "utils/my_lr_select.hpp"
#include "utils/ext_base_menu_page.hpp"
#include "utils/my_menu_simple_message.hpp"

#include "Sequence/ControlSlider.hpp"
#include "Sequence/BaseMenuPage.hpp"
#include "Sequence/FaderPage.hpp"
#include "UI/BackButton.hpp"
#include "UI/CursorMove.hpp"

mod::utils::MyLRSelect *lr_select_array[4] = {0};

enum ClassLRSelect_OnOff : u32 {
    OPTION_OFF,
    OPTION_ON
};

// engine_class_select
enum ClassLRSelect_EngineClass : u32 {
    _150_CC,
    _300_CC,
    _500_CC,
    _9999_CC
};

mod::utils::MyLRSelect::Settings engine_class_select_settings(
    0,
    u"Modded 150cc",
    {
        { u"No", u"300cc", u"500cc", u"9999cc" },
        4
    }
);

void onApply_engineClassSelect(mod::utils::MyLRSelect *lr_select) {
    extern f32 EngineClassSpeedMultipliers[4];

    switch (lr_select->m_option) {
        case _150_CC:
        default:
            EngineClassSpeedMultipliers[0] = 1.0f;
            break;

        case _300_CC:
            EngineClassSpeedMultipliers[0] = 3.5f;
            break;

        case _500_CC:
            EngineClassSpeedMultipliers[0] = 6.0f;
            break;

        case _9999_CC:
            EngineClassSpeedMultipliers[0] = 100.0f;
            break;
    }
}

// unused_leaf_type_select
enum ClassLRSelect_LeafType : u32 {
    TYPE_NORMAL,
    TYPE_UNUSED
};

mod::utils::MyLRSelect::Settings unused_leaf_type_select_settings(
    0,
    u"Leaf Type",
    {
        { u"Normal", u"Unused" },
        2
    }
);

void onApply_unusedLeafTypeSelect(mod::utils::MyLRSelect *lr_select) {
    extern s32 ItemObjTail_defaultOnUseTailType;

    switch (lr_select->m_option) {
        case TYPE_NORMAL:
        default:
            ItemObjTail_defaultOnUseTailType = 3;
            break;

        case TYPE_UNUSED:
            ItemObjTail_defaultOnUseTailType = 0;
            break;
    }
}

// ultra_miniturbo_select
bool g_ultra_miniturbo_enabled = false;

mod::utils::MyLRSelect::Settings ultra_miniturbo_select_settings(
    0,
    u"Ultra Miniturbo",
    {
        { u"Off", u"On" },
        2
    }
);

void onApply_ultraMiniturbo(mod::utils::MyLRSelect *lr_select) {
    switch (lr_select->m_option) {
        case OPTION_OFF:
        default:
            g_ultra_miniturbo_enabled = false;
            break;

        case OPTION_ON:
            g_ultra_miniturbo_enabled = true;
            break;
    }
}

// random_stats
bool g_random_stats_enabled = false;

mod::utils::MyLRSelect::Settings random_stats_select_settings(
    0,
    u"Random stats",
    {
        { u"Off", u"On" },
        2
    }
);

void onApply_randomStats(mod::utils::MyLRSelect *lr_select) {
    switch (lr_select->m_option) {
        case OPTION_OFF:
        default:
            g_random_stats_enabled = false;
            break;

        case OPTION_ON:
            g_random_stats_enabled = true;
            break;
    }
}

/////////////////

HOOK void classLRSelect_initControl(Sequence::BaseMenuPage *menu) {
    // Create the upper screen message window
    mod::utils::MyMenuSimpleMessage *simple_message = mod::utils::setupControl<mod::utils::MyMenuSimpleMessage>(menu, "dialogue01", "dialogue");
    simple_message->setMessage(u"Mod Menu");

    // Create the back button
    UI::BackButton *back_button = menu->setupControl<UI::BackButton>("cmn_back_btn", "cmn_back_btn");
    back_button->m_on_button_press_se = Sound::SndSeEvent::EEvent::SE_SYS_CANCEL_L;
    back_button->m_return_code = menu->m_on_back_return_code;

    // Create each LRSelect
    // engine_class_select 
    lr_select_array[0] = mod::utils::MyLRSelect::createLRSelect(menu, false, mod::utils::MyLRSelect::EDesign::DESIGN_0);
    lr_select_array[0]->initSettings(engine_class_select_settings);
    lr_select_array[0]->setOnApply(onApply_engineClassSelect);
    lr_select_array[0]->initCaption(menu, false, u"Select a custom top speed for 150cc");
    // TODO: This doesn't work for `engine_class_select`. Only for `unused_leaf_type_select`
    lr_select_array[0]->setPosY(40.0f);

    // unused_leaf_type_select
    lr_select_array[1] = mod::utils::MyLRSelect::createLRSelect(menu, false, mod::utils::MyLRSelect::EDesign::DESIGN_0);
    lr_select_array[1]->initSettings(unused_leaf_type_select_settings);
    lr_select_array[1]->setOnApply(onApply_unusedLeafTypeSelect);
    lr_select_array[1]->initCaption(menu, false, u"Enables unused Super Leaf behaviour");
    lr_select_array[1]->setPosY(00.0f);

    // ultra_miniturbo_select
    lr_select_array[2] = mod::utils::MyLRSelect::createLRSelect(menu, false, mod::utils::MyLRSelect::EDesign::DESIGN_0);
    lr_select_array[2]->initSettings(ultra_miniturbo_select_settings);
    lr_select_array[2]->setOnApply(onApply_ultraMiniturbo);
    lr_select_array[2]->initCaption(menu, false, u"Enables purple miniturbos");
    lr_select_array[2]->setPosY(-40.0f);

    // random_stats
    lr_select_array[3] = mod::utils::MyLRSelect::createLRSelect(menu, false, mod::utils::MyLRSelect::EDesign::DESIGN_0);
    lr_select_array[3]->initSettings(random_stats_select_settings);
    lr_select_array[3]->setOnApply(onApply_randomStats);
    lr_select_array[3]->initCaption(menu, false, u"Randomizes your vehicle stats on each race");
    lr_select_array[3]->setPosY(-80.0f);

    // TODO: Do we really need to call this?
    menu->m_manipulators[0]->m_cursor_move.setType(UI::CursorMove::EType::NEXT_GAME_SETTING);
}

HOOK void classLRSelect_onPageEnter(Sequence::BaseMenuPage *menu) {
    lr_select_array[0]->selectHandlerOn(0, 0);   // Ensure the caption for the first select appears when entering the menu
    
    Sequence::StartFadein(Sequence::Fader::EFaderType::FADE_IN_BLACK, 30, Sequence::Fader::EFaderScreen::BOTH_SCREENS);
}

HOOK void classLRSelect_onPageComplete(Sequence::BaseMenuPage *menu) {
    Sequence::StartFadeout(Sequence::Fader::EFaderType::FADE_OUT_BLACK, 30, Sequence::Fader::EFaderScreen::BOTH_SCREENS);
}
