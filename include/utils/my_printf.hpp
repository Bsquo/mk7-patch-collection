#pragma once

#include "Sequence/BasePage.hpp"
#include "UI/ControlAnimator.hpp"
#include "UI/MenuCaption.hpp"
#include <nw/lyt/TextBox.hpp>

namespace mod {
namespace utils {

class MyPrintf: public UI::MenuCaption {
    public:
        struct AnimationDefine: public UI::MenuCaption::AnimationDefine {};

        struct CreateArg: public UI::MenuCaption::CreateArg {
            void _0x0() {}
            void _0x8() {}
            const UI::ControlAnimator::AnimationDefine *getAnimationDefine() const { return &m_animation_define; }
        };

        void _0x2C() {}
        void _0x38() {}
        void _0x44() {}

        MyPrintf() {}
        ~MyPrintf() {}
        static MyPrintf *createPrint(Sequence::BasePage *menu);
        void setMessage(const UI::MessageString &);
        void setMessage(u32);
        void setColor(const nw::ut::Color8, s32);
        void setPos(f32, f32);
        void setOutlineColor(u8);
        void setSize(f32, f32);
        void setLineSpace(f32);
        void setCharSpace(f32);
        void setTextAlignment(u8);
        void setDrawBottomScreen(bool);
        void printf(f32, f32, const wchar_t *, ...);
        void hideBackground();
        void reallocateStringBuffer();

    private:
        static const u16 BUFFER_CHARS = 1024;
        nw::lyt::TextBox *getTextBox() const;
        UI::MessageString formatMessage(wchar_t *, u16, const wchar_t *, va_list);
        // One string buffer per `MyPrintf` instance
        wchar_t m_wbuf[BUFFER_CHARS];
};

}
}