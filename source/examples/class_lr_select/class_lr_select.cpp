#include "mod_common.hpp"
#include "examples/class_lr_select/class_lr_select.hpp"
#include "utils/my_printf.hpp"
#include "utils/my_lr_select_group.hpp"
#include "utils/ext_base_menu_page.hpp"
#include "utils/my_menu_simple_message.hpp"

#include "Sequence/ControlSlider.hpp"
#include "Sequence/BaseMenuPage.hpp"
#include "Sequence/FaderPage.hpp"
#include "Sound/SndEngine.hpp"
#include "Sound/SndSceneMenu.hpp"
#include "System/RootSystem.hpp"
#include "UI/BackButton.hpp"
#include "UI/CursorMove.hpp"

#include <controller/ctr/seadCtrController.h>

mod::utils::MyLRSelectGroup *lr_select_group = nullptr;
mod::utils::MyPrintf *page_numbers = nullptr;
mod::utils::MyPrintf *category = nullptr;

/////////////////

const wchar_t* category_names[] = {
    L"Misc",
    L"Misc 2",
    L"Enhancements",
    L"Sound"
};

/////////////////

// engine_class_select
mod::utils::MyLRSelect::Settings engine_class_select_settings(
    _150_CC,
    u"Modded 150cc",
    {
        { u"No", u"300cc", u"500cc", u"9999cc" },
        4
    },
    u"Modify the engine speed in 150cc"
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
mod::utils::MyLRSelect::Settings unused_leaf_type_select_settings(
    TYPE_NORMAL,
    u"Leaf Type",
    {
        { u"Normal", u"Unused" },
        2
    },
    u"Enable the unused Super Leaf behaviour"
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
    OPTION_OFF,
    u"Ultra Miniturbo",
    
    {
        { u"Off", u"On" },
        2
    },
    u"Enable purple miniturbos"
);

void onApply_ultraMiniturbo(mod::utils::MyLRSelect *lr_select) {
    g_ultra_miniturbo_enabled = lr_select->m_option;
}

// random_stats
bool g_random_stats_enabled = false;

mod::utils::MyLRSelect::Settings random_stats_select_settings(
    OPTION_OFF,
    u"Random stats",
    {
        { u"Off", u"On" },
        2
    },
    u"Randomize the vehicle statistics on each race"
);

void onApply_randomStats(mod::utils::MyLRSelect *lr_select) {
    g_random_stats_enabled = lr_select->m_option;
}

// race_prints
u32 g_race_prints_option = RACE_PRINTS_OFF;

mod::utils::MyLRSelect::Settings race_prints_select_settings(
    RACE_PRINTS_SPEED,
    u"Race prints",
    {
        { u"Off", u"Speed", u"Speed (XYZ)", u"Full" },
        4
    },
    u"Display one of the following during races:\n\n"
    u" - Speed: Shows the engine's speed\n"
    u" - Speed (XYZ): Shows the speed including forces\n"
    u" - Full: Shows the kart and race status"
);

void onApply_racePrints(mod::utils::MyLRSelect *lr_select) {
    g_race_prints_option = lr_select->m_option;
}

// input_viewer
u32 g_input_viewer_option = INPUT_VIEWER_OFF;

mod::utils::MyLRSelect::Settings input_viewer_select_settings(
    INPUT_VIEWER_NO_BG,
    u"Input viewer",
    {
        { u"No", u"No background", u"With background"},
        3
    },
    u"Shows an input viewer during races"
);

void onApply_inputViewerSelect(mod::utils::MyLRSelect *lr_select) {
    g_input_viewer_option = lr_select->m_option;
}

// variable_mii_size
u32 g_variable_mii_size = OPTION_OFF;

mod::utils::MyLRSelect::Settings variable_mii_size_select_settings(
    OPTION_OFF,
    u"Mii size",
    {
        { u"Normal", u"Variable"},
        2
    },
    u" - Normal: Miis are always medium weight\n\n"
    u" - Variable: Miis can also be large or\n"
    u"   small depending on its size,\n"
    u"   just like in MKWii\n"
);

void onApply_variableMiiSize(mod::utils::MyLRSelect *lr_select) {
    g_variable_mii_size = lr_select->m_option;
}

// disable_cpu_rubberbanding
mod::utils::MyLRSelect::Settings disable_cpu_rubberbanding_select_settings(
    OPTION_OFF,
    u"CPU Rubberbanding",
    {
        { u"Enabled", u"Disabled"},
        2
    },
    u"Allow CPUs to catch up or slow down\n"
    u"depending on how far you're from them"
);

void onApply_disableCPURubberbanding(mod::utils::MyLRSelect *lr_select) {
    if (lr_select->m_option == 1) {
        PATCH_INSTR(ADDR_DISABLE_CPU_RUBBERBANDING, 0xE12FFF1E);  // bx lr
    }
    else {
        PATCH_INSTR(ADDR_DISABLE_CPU_RUBBERBANDING, 0xEEB40A40);  // Original instruction
    }
}

// drift_from_standstill
mod::utils::MyLRSelect::Settings drift_from_standstill_select_settings(
    OPTION_OFF,
    u"Drift from stop",
    {
        { u"No", u"Yes"},
        2
    },
    u"Allows you to drift without having to\n"
    u"wait reaching 55%% of your max speed"
);

void onApply_driftFromStandstill(mod::utils::MyLRSelect *lr_select) {
    extern f32 MIN_DRIFT_SPEED_PERCENTAGE;

    MIN_DRIFT_SPEED_PERCENTAGE = (lr_select->m_option) ? 0.0f : 0.55f;
}

// disable_bgm
mod::utils::MyLRSelect::Settings disable_bgm_select_settings(
    OPTION_OFF,
    u"BGM",
    {
        { u"Yes", u"No"},
        2
    },
    u"Toggle background music"
);

void onApply_disableBGM(mod::utils::MyLRSelect *lr_select) {
    if (lr_select->m_option == 1) {
        PATCH_INSTR(ADDR_DISABLE_BGM_1, 0xE12FFF1E);  // bx lr
        PATCH_INSTR(ADDR_DISABLE_BGM_2, 0xE12FFF1E);  // bx lr
        PATCH_INSTR(ADDR_DISABLE_BGM_3, 0xE12FFF1E);  // bx lr
        PATCH_INSTR(ADDR_DISABLE_BGM_4, 0xE12FFF1E);  // bx lr
    }
    else {
        // Original instructions
        PATCH_INSTR(ADDR_DISABLE_BGM_1, 0xE92D41F0);
        PATCH_INSTR(ADDR_DISABLE_BGM_2, 0xE92D4010);
        PATCH_INSTR(ADDR_DISABLE_BGM_3, 0xE5D0303C);
        PATCH_INSTR(ADDR_DISABLE_BGM_4, 0xE2800004);
    }
}

// disable_voices
u32 g_disable_voices = OPTION_OFF;

mod::utils::MyLRSelect::Settings disable_voices_select_settings(
    OPTION_OFF,
    u"Voices",
    {
        { u"Yes", u"No"},
        2
    },
    u"Toggle character voices"
);

void onApply_disableVoices(mod::utils::MyLRSelect *lr_select) {
    g_disable_voices = lr_select->m_option;

    if (g_disable_voices) {
        PATCH_INSTR(ADDR_DISABLE_VOICES_4, 0xE12FFF1E);  // bx lr
        PATCH_INSTR(ADDR_DISABLE_VOICES_5, 0xE12FFF1E);  // bx lr
        PATCH_INSTR(ADDR_DISABLE_VOICES_6, 0xE12FFF1E);  // bx lr
    }
    else {
        // Original instructions
        PATCH_INSTR(ADDR_DISABLE_VOICES_4, 0xE92D41F0);  // bx lr
        PATCH_INSTR(ADDR_DISABLE_VOICES_5, 0xE92D41F0);  // bx lr
        PATCH_INSTR(ADDR_DISABLE_VOICES_6, 0xE92D4070);  // bx lr
    }
}

// high_poly_racers
mod::utils::MyLRSelect::Settings high_poly_racers_select_settings(
    OPTION_OFF,
    u"High Poly CPUs",
    {
        { u"No", u"Yes"},
        2
    },
    u"Makes CPU racers use their\n"
    u"high poly models (WIP)"
);

void onApply_highPolyRacers(mod::utils::MyLRSelect *lr_select) {
    if (lr_select->m_option == 1) {
        PATCH_INSTR(ADDR_HIGH_POLY_RACERS_1, 0xE1A00000);
        PATCH_INSTR(ADDR_HIGH_POLY_RACERS_2, 0xE1A00000);
        PATCH_INSTR(ADDR_HIGH_POLY_RACERS_3, 0xE3A00001);
        PATCH_INSTR(ADDR_HIGH_POLY_RACERS_4, 0xEA000007);
    }
    else {
        // Original instructions
        PATCH_INSTR(ADDR_HIGH_POLY_RACERS_1, 0x1A000000);
        PATCH_INSTR(ADDR_HIGH_POLY_RACERS_2, 0x1A000000);
        PATCH_INSTR(ADDR_HIGH_POLY_RACERS_3, 0xE5D5001A);
        PATCH_INSTR(ADDR_HIGH_POLY_RACERS_4, 0x0A000007);
    }
}

// no_oob_respawn
mod::utils::MyLRSelect::Settings no_oob_respawn_select_settings(
    OPTION_OFF,
    u"No OoB Respawn",
    {
        { u"Disabled", u"Enabled"},
        2
    },
    u"You won't respawn when leaving\n"
    u"a checkpoint area"
);

void onApply_noOobRespawn(mod::utils::MyLRSelect *lr_select) {
    if (lr_select->m_option == 1) {
        PATCH_INSTR(ADDR_NO_OOB_RESPAWN, 0xEA000002);
    }
    else {
        // Original instructions
        PATCH_INSTR(ADDR_NO_OOB_RESPAWN, 0x0A000002);
    }
}

/////////////////

HOOK void classLRSelect_initControl(Sequence::BaseMenuPage *menu) {
    const u32 num_pages = 4;

    lr_select_group = new mod::utils::MyLRSelectGroup();
    lr_select_group->initControl(menu, false, true, false);
    lr_select_group->initSettings(num_pages);
    lr_select_group->setCurrentPage(0);

    // Page 1: Misc
    // engine_class_select
    lr_select_group->setupEntry(0, 0, &engine_class_select_settings, onApply_engineClassSelect);
    // unused_leaf_type_select
    lr_select_group->setupEntry(1, 0, &unused_leaf_type_select_settings, onApply_unusedLeafTypeSelect);
    // ultra_miniturbo_select
    lr_select_group->setupEntry(2, 0, &ultra_miniturbo_select_settings, onApply_ultraMiniturbo);
    // random_stats
    lr_select_group->setupEntry(3, 0, &random_stats_select_settings, onApply_randomStats);

    // Page 2: Misc 2
    // variable_mii_size
    lr_select_group->setupEntry(0, 1, &variable_mii_size_select_settings, onApply_variableMiiSize);
    // disable_cpu_rubberbanding
    lr_select_group->setupEntry(1, 1, &disable_cpu_rubberbanding_select_settings, onApply_disableCPURubberbanding);
    // drift_from_standstill
    lr_select_group->setupEntry(2, 1, &drift_from_standstill_select_settings, onApply_driftFromStandstill);
    // no_oob_respawn
    lr_select_group->setupEntry(3, 1, &no_oob_respawn_select_settings, onApply_noOobRespawn);

    // Page 3: Enhancements
    // race_prints
    lr_select_group->setupEntry(0, 2, &race_prints_select_settings, onApply_racePrints);
    // input_viewer
    lr_select_group->setupEntry(1, 2, &input_viewer_select_settings, onApply_inputViewerSelect);
    // high_poly_racers
    lr_select_group->setupEntry(2, 2, &high_poly_racers_select_settings, onApply_highPolyRacers);

    // Page 4: Sound
    // disable_bgm
    lr_select_group->setupEntry(0, 3, &disable_bgm_select_settings, onApply_disableBGM);

    // disable_voices
    lr_select_group->setupEntry(1, 3, &disable_voices_select_settings, onApply_disableVoices);

    lr_select_group->initCurrentPage();

    // Create the back button
    UI::BackButton *back_button = menu->setupControl<UI::BackButton>("cmn_back_btn", "cmn_back_btn");
    back_button->m_on_button_press_se = Sound::SndSeEvent::EEvent::SE_SYS_CANCEL_L;
    back_button->m_return_code = menu->m_on_back_return_code;

    // Create the page number prints
    page_numbers = mod::utils::MyPrintf::createPrint(menu, true);
    page_numbers->setTextAlignment(nw::lyt::ALIGN_TOP_LEFT);
    page_numbers->setSize(16.0f, 16.0f);

    // Create the category name print
    category = mod::utils::MyPrintf::createPrint(menu, true);
    category->setTextAlignment(nw::lyt::ALIGN_TOP_LEFT);
    category->setSize(16.0f, 16.0f);

}

HOOK void classLRSelect_onPageEnter(Sequence::BaseMenuPage *menu) {
    Sequence::StartFadein(Sequence::Fader::EFaderType::FADE_IN_BLACK, 30, Sequence::Fader::EFaderScreen::BOTH_SCREENS);
}

HOOK void classLRSelect_onPagePreStep(Sequence::BaseMenuPage *menu) {
    lr_select_group->calc();

    page_numbers->printf
    (
        280.0f, 20.0f,
        L"%3d / %3d",
        lr_select_group->getCurrentPage() + 1, lr_select_group->getNumPages()
    );

    category->printf
    (
        40.0f, 20.0f,
        category_names[lr_select_group->getCurrentPage()]
    );
}

HOOK void classLRSelect_onPageComplete(Sequence::BaseMenuPage *menu) {
    Sequence::StartFadeout(Sequence::Fader::EFaderType::FADE_OUT_BLACK, 30, Sequence::Fader::EFaderScreen::BOTH_SCREENS);
    System::g_root_system->m_root_scene->get_sound_engine()->m_director_list->m_snd_scene_menu->stopSceneBgm(30);
}
