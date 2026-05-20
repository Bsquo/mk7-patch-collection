#pragma once

#include "forward.hpp"
#include "UI/BaseMenuViewControl.hpp"


#include <nw/lyt/Pane.hpp>
#include <nw/lyt/TextBox.hpp>
#include <prim/seadSafeString.hpp>

namespace mod {

class InputViewer: public UI::BaseMenuViewControl {
    public:
        enum { NUM_OFF_ON_INPUTS = 6 };

        enum EInput : u32 {
            INPUT_A,
            INPUT_B,
            INPUT_X,
            INPUT_Y,
            INPUT_L,
            INPUT_R,

            INPUT_STICK,
            INPUT_STICK_OUTER,
            INPUT_STICK_TEXTBOX
        };

        enum EInputState : u32 {
            OFF,
            ON
        };

        struct AnimationDefine: public UI::ControlAnimator::AnimationDefine {
            void defineAnimation() {}
        };

        struct CreateArg: public UI::VisualControl::CreateArg {
            void _0x0() {}
            void _0x8() {}
            const UI::ControlAnimator::AnimationDefine *getAnimationDefine() const { return &m_animation_define; }
        };

        void _0x2C() {}
        void _0x38() {}
        void _0x44() {}

        InputViewer() = default;
        virtual ~InputViewer() = default;
        virtual void onCreate(const Control::CreateArg *);
        virtual void onReset();
        virtual void onCalc();

        private:
            nw::lyt::Pane *getElement(const sead::SafeString &, const UI::ControlSight::EElementType);
            void buttonOff(u32);
            void buttonOn(u32);

            nw::lyt::Pane *m_button_panes[NUM_OFF_ON_INPUTS][2];
            u32 m_buttons[NUM_OFF_ON_INPUTS];
            nw::lyt::Pane *m_stick_pane;
            nw::lyt::TextBox * m_stick_text;
};

}
