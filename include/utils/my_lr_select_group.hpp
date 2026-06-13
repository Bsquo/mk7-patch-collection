#pragma once

#include "my_lr_select.hpp"
#include "my_menu_simple_message.hpp"

#include <container/seadPtrArray.h>

namespace mod {
namespace utils {

class MyLRSelectGroup {
    public:
        struct Settings {
            bool m_is_active = false;
            MyLRSelect::Settings *m_entry_settings;
            MyLRSelect::OnApplyCallback m_on_apply;
            s32 m_selected_option = 0;
        };

        static constexpr s32 NUM_ENTRIES = 5;

        MyLRSelectGroup() { disableAllEntries(); }
        void initControl(Sequence::BaseMenuPage *, bool, bool, bool do_hide_background = false);
        void setNumPages(s32 num_pages) { m_num_pages = num_pages; }
        s32 getNumPages() const { return m_num_pages; }
        void setCurrentPage(s32 current_page) { m_current_page = current_page; }
        s32 getCurrentPage() const { return m_current_page; }
        void initSettings(s32);
        MyLRSelect *getEntry(s32 i) { return m_entries[i]; }
        Settings *getSettings(s32 entry, s32 page) { return &m_settings[entry][page]; }
        void setupEntry(s32, s32, MyLRSelect::Settings *, MyLRSelect::OnApplyCallback);
        void initCurrentPage();
        void calc();

    private:
        void disableAllEntries();
        void selectEntry(s32);

        MyLRSelect *m_entries[NUM_ENTRIES] = {nullptr};
        MyLRSelectBg *m_bg = nullptr;
        MyMenuSimpleMessage *m_simple_message = nullptr;
        Sequence::BaseMenuPage *m_menu = nullptr;
        Settings **m_settings;
        s32 m_current_page = -1;
        s32 m_num_pages = -1;
};

}
}