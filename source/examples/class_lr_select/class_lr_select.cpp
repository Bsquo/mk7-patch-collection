#include "common.hpp"
#include "utils/my_lr_select.hpp"
#include "Sequence/ControlSlider.hpp"
#include "Sequence/BaseMenuPage.hpp"

mod::utils::MyLRSelect *engine_class_select = nullptr;

//// Settings. TODO: Cleanup ////

enum ClassLRSelect_EngineClass : u32 {
    _150_CC,
    _300_CC,
    _500_CC,
    _9999_CC
};

static char16_t name_str[] = u"CC";  
static UI::MessageString name{name_str};

static char16_t opt0_str[] = u"150cc";
static UI::MessageString opt0{opt0_str};

static char16_t opt1_str[] = u"300cc";  
static UI::MessageString opt1{opt1_str};

static char16_t opt2_str[] = u"500cc";  
static UI::MessageString opt2{opt2_str};

static char16_t opt3_str[] = u"9999cc";  
static UI::MessageString opt3{opt3_str};

static UI::MessageString options[] = { opt0, opt1, opt2, opt3 };

mod::utils::MyLRSelect::Settings engine_class_select_settings(
    ARRAY_COUNT(options),      // num_options
    0,                         // default_option  
    name,
    options
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

/////////////////

HOOK void classLRSelect_initControl() {
    Sequence::BaseMenuPage *menu;
    READ_ARM_REG(r4, menu);

    engine_class_select = mod::utils::MyLRSelect::createLRSelect(menu, true);
    engine_class_select->initSettings(engine_class_select_settings);
    engine_class_select->setOnApply(onApply_engineClassSelect);
}
