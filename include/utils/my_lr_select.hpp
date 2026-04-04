#pragma once

#include "Sequence/BasePage.hpp"
#include "UI/ControlAnimator.hpp"
#include "UI/LRSelect.hpp"
#include "UI/LRSelectBg.hpp"

namespace mod {
namespace utils {

class MyLRSelect: public UI::LRSelect {
    public:
        struct AnimationDefine: public UI::LRSelect::AnimationDefine {};

        struct CreateArg: public UI::LRSelect::CreateArg {
            void _0x0() {}
            void _0x8() {}
            const UI::ControlAnimator::AnimationDefine *getAnimationDefine() const { return &m_animation_define; }
        };

        struct Settings {
            s32 m_num_options;
            s32 m_default_option;
            UI::MessageString name;
            UI::MessageString *options_text;

            s32 name_message_id = -1;
            s32 *options_message_ids = nullptr;

            Settings()
                : m_num_options(0),
                m_default_option(-1),
                name(nullptr),
                options_text(nullptr),
                name_message_id(-1),
                options_message_ids(nullptr) {}

            Settings(s32 num_options,
                    s32 default_option,
                    UI::MessageString name_,
                    UI::MessageString *options_text_)
                : m_num_options(num_options),
                m_default_option(default_option),
                name(name_),
                options_text(options_text_) {}

            Settings(s32 num_options,
                    s32 default_option,
                    s32 name_msg_id,
                    s32* options_msg_ids)
                : m_num_options(num_options),
                m_default_option(default_option),
                name_message_id(name_msg_id),
                options_message_ids(options_msg_ids) {}

            ~Settings() = default;

            Settings& operator=(const Settings& other) {
                if (this != &other) {
                    m_num_options = other.m_num_options;
                    m_default_option = other.m_default_option;
                    name = other.name;
                    options_text = other.options_text;
                    name_message_id = other.name_message_id;
                    options_message_ids = other.options_message_ids;
                }
                return *this;
            }
        };

        using OnApplyCallback = void (*)(MyLRSelect*);

        void _0x2C() {}
        void _0x38() {}
        void _0x44() {}

        MyLRSelect() {
            id = num_lr_select;
            num_lr_select++;
        }
        ~MyLRSelect() {
            id = -1;
            num_lr_select--;
        }
        void onReset();
        void keyHandlerCursor(s32, s32);

        static MyLRSelect *createLRSelect(Sequence::BasePage *, bool);
        void initSettings(const Settings &);
        void setOnApply(OnApplyCallback callback);

        Settings m_settings;

        static s32 num_lr_select;
        // In this array, we'll save the options selected, so that they don't get lost
        // when the scene changes. Each entry in the array corresponds to a separate LRSelect (identified by the `id` member),
        // and its value will be the selected `m_option`.

        // DashSequenceEngine already has an array for this purpose (field 0xE8), but it only supports up to 23 entries.
        static s32 options_array[100];

    private:
        void updateSelection();
        OnApplyCallback onApply = nullptr;
        s32 id = -1;
};

class MyLRSelectBg: public UI::LRSelectBg {
    public:
        struct AnimationDefine: public UI::LRSelectBg::AnimationDefine {};

        struct CreateArg: public UI::LRSelectBg::CreateArg {
            void _0x0() {}
            void _0x8() {}
            const UI::ControlAnimator::AnimationDefine *getAnimationDefine() const { return &m_animation_define; }
        };

        void _0x2C() {}
        void _0x38() {}
        void _0x44() {}

        MyLRSelectBg() = default;
};

}
}