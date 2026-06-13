#include "mod_common.hpp"
#include "utils/my_lr_select_group.hpp"
#include "utils/ext_base_menu_page.hpp"

#include "Sequence/PlaySe.hpp"
#include "System/KDPadDirector.hpp"
#include "System/KDPadControllerCore.hpp"
#include "System/RootSystem.hpp"
#include "System/SystemEngine.hpp"

namespace mod {
namespace utils {

void MyLRSelectGroup::initControl(Sequence::BaseMenuPage *menu_page, bool use_caption, bool use_menu_simple_message, bool do_hide_background) {
    MyPrintf *caption = nullptr;
    MyMenuSimpleMessage *menu_simple_message = nullptr;
    m_menu = menu_page;
    
    // Create the background
    m_bg = mod::utils::setupControl<MyLRSelectBg>(m_menu, "game_setup_bg", "game_setup_bg");

    if (use_caption)
        caption = MyPrintf::createPrint(m_menu, do_hide_background);

    if (use_menu_simple_message) {
        menu_simple_message = mod::utils::setupControl<mod::utils::MyMenuSimpleMessage>(m_menu, "dialogue01", "dialogue"); 
        menu_simple_message->setTextboxSize(16.0f, 16.0f);
    }
    // Create each LRSelect
    m_entries[0] = mod::utils::MyLRSelect::createLRSelect(m_menu, false, mod::utils::MyLRSelect::EDesign::DESIGN_0, "game_setup_5_00");
    m_entries[1] = mod::utils::MyLRSelect::createLRSelect(m_menu, false, mod::utils::MyLRSelect::EDesign::DESIGN_0, "game_setup_5_01");
    m_entries[2] = mod::utils::MyLRSelect::createLRSelect(m_menu, false, mod::utils::MyLRSelect::EDesign::DESIGN_0, "game_setup_5_02");
    m_entries[3] = mod::utils::MyLRSelect::createLRSelect(m_menu, false, mod::utils::MyLRSelect::EDesign::DESIGN_0, "game_setup_5_03");
    m_entries[4] = mod::utils::MyLRSelect::createLRSelect(m_menu, false, mod::utils::MyLRSelect::EDesign::DESIGN_0, "game_setup_5_04");
    
    for (s32 i = 0; i < NUM_ENTRIES; i++) {
        m_entries[i]->setBg(m_bg);
        m_entries[i]->initDescription(use_caption, use_menu_simple_message, caption, menu_simple_message, menu_page, do_hide_background);
    }

    // TODO: Do we really need to call this?
    m_menu->m_manipulators[0]->m_cursor_move.setType(UI::CursorMove::EType::NEXT_GAME_SETTING);
}

void MyLRSelectGroup::initSettings(s32 num_pages) {
    setNumPages(num_pages);

    m_settings = new Settings*[NUM_ENTRIES];
    for (s32 i = 0; i < NUM_ENTRIES; i++) {
        m_settings[i] = new Settings[m_num_pages];
    }
}

void MyLRSelectGroup::setupEntry(s32 entry_index, s32 page, MyLRSelect::Settings *entry_settings, MyLRSelect::OnApplyCallback on_apply) {
    Settings *settings = getSettings(entry_index, page);
    
    settings->m_is_active = true;
    settings->m_entry_settings = entry_settings;
    settings->m_on_apply = on_apply;
}

void MyLRSelectGroup::initCurrentPage() {
    Settings *settings = nullptr;

    for (s32 i = 0; i < NUM_ENTRIES; i++) {
        settings = getSettings(i, m_current_page);
        
        if (settings->m_is_active) {
            m_entries[i]->m_cursor_item.m_is_enabled = true;
            m_entries[i]->m_visible = true;
            m_entries[i]->initSettingsWithSpecificOption(*settings->m_entry_settings, settings->m_selected_option);
            m_entries[i]->setOnApply(settings->m_on_apply);
        }
        else {
            m_entries[i]->m_cursor_item.m_is_enabled = false;
            m_entries[i]->m_visible = false;
        }
    }

    // Select the first option when switching pages
    m_entries[0]->m_cursor_item.m_manipulator->m_selected_option = 0;   // TODO: This line doesn't seem to do anything
    m_entries[0]->selectHandlerOn(0, 0);
}

void MyLRSelectGroup::calc() {
    Settings *settings = nullptr;
    System::KDPadControllerCore *controller = System::g_root_system->get_pad_director()->m_pads[static_cast<s32>(System::KDPadDirector::eKDPadListIndex::UI)]->m_pad_controller->m_pad_controller_core;

    if (controller->mPadTrig & sead::CtrController::BUTTON_R) {
        setCurrentPage((m_current_page + 1 < m_num_pages) ? m_current_page + 1 : 0);
        Sequence::PlaySe(Sound::SndSeEvent::EEvent::SE_SYS_CURSOR_1);
        initCurrentPage();
    }
    else if (controller->mPadTrig & sead::CtrController::BUTTON_L) {
        setCurrentPage((m_current_page > 0) ? m_current_page - 1 : m_num_pages - 1);
        Sequence::PlaySe(Sound::SndSeEvent::EEvent::SE_SYS_CURSOR_1);
        initCurrentPage();
    }

    for (s32 i = 0; i < NUM_ENTRIES; i++) {
        settings = getSettings(i, m_current_page);

        if (settings->m_is_active) {
            settings->m_selected_option = m_entries[i]->m_option;
        }
    }
}

void MyLRSelectGroup::disableAllEntries() {
    if (m_num_pages > 0) {
        for (s32 i = 0; i < NUM_ENTRIES; i++) {
            for (s32 j = 0; j < m_num_pages; j++) {
                getSettings(i, j)->m_is_active = false;
            }
        }
    }
}

}
}