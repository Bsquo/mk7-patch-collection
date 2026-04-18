#pragma once

#include "utils/my_printf.hpp"

#include "Sequence/BasePage.hpp"
#include "UI/ControlAnimator.hpp"
#include "UI/LRSelect.hpp"
#include "UI/LRSelectBg.hpp"

namespace mod {
namespace utils {

class MyLRSelect: public UI::LRSelect {
    public:
        enum EDesign: u32 {
            // Name and selection have the same size
            DESIGN_0,
            // Name is significantly smaller than selection
            DESIGN_1,
            // Name is located above the selection
            DESIGN_2
        };

        struct AnimationDefine: public UI::LRSelect::AnimationDefine {};

        struct CreateArg: public UI::LRSelect::CreateArg {
            void _0x0() {}
            void _0x8() {}
            const UI::ControlAnimator::AnimationDefine *getAnimationDefine() const { return &m_animation_define; }
        };

        struct Settings {
            static const s32 MAX_OPTIONS = 10;

            s32 m_num_options;
            s32 m_default_option;
            UI::MessageString name;
            UI::MessageString options_text[MAX_OPTIONS];
            s32 name_message_id = -1;

            struct OptionList {
                const char16_t* data[MAX_OPTIONS];
                s32 count;
            };

            Settings(
                s32 default_option,
                const char16_t* name_str,
                OptionList list
            ):
                m_num_options(list.count),
                m_default_option(default_option),
                name(name_str)
            {
                for (s32 i = 0; i < m_num_options; i++) {
                    options_text[i] = UI::MessageString(list.data[i]);
                }
            }

            Settings(): m_num_options(0), m_default_option(0), name(nullptr) {}

            ~Settings() = default;
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
            
            // TODO: Figure out why this crashes
            //if (caption != nullptr) {
            //    delete caption;
            //}

            caption = nullptr;
        }
        void onReset();
        void keyHandlerCursor(s32, s32);
        void selectHandlerOn(s32, s32);
        void selectHandlerOff(s32, s32);

        static MyLRSelect *createLRSelect(Sequence::BasePage *, bool, EDesign);
        void initSettings(const Settings &);
        void setOnApply(OnApplyCallback callback);
        void initCaption(Sequence::BasePage *, bool, const UI::MessageString &);
        void setPosY(f32);

        Settings m_settings;

        static s32 num_lr_select;
        // In this array, we'll save the options selected, so that they don't get lost
        // when the scene changes. Each entry in the array corresponds to a separate LRSelect (identified by the `id` member),
        // and its value will be the selected `m_option`.

        // DashSequenceEngine already has an array for this purpose (field 0xE8), but it only supports up to 23 entries.
        static s32 options_array[100];
        MyPrintf *caption = nullptr;

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