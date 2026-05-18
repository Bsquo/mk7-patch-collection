#pragma once

#include "forward.hpp"
#include "UI/BaseMenuViewControl.hpp"
#include <controller/ctr/seadCtrController.h>

namespace mod {

class InputViewerButton: public UI::BaseMenuViewControl {
    public:
        static const f32 ANIM_RELEASE;
        static const f32 ANIM_HOLD;

        enum class EType : u32 {
            BUTTON_CIRCLE,
            BUTTON_OVAL
        };

        struct AnimationDefine: public UI::ControlAnimator::AnimationDefine {
            void defineAnimation();
        };

        struct CreateArg: public UI::VisualControl::CreateArg {
            void _0x0() {}
            void _0x8() {}
            const UI::ControlAnimator::AnimationDefine *getAnimationDefine() const { return &m_animation_define; }
        };

        void _0x2C() {}
        void _0x38() {}
        void _0x44() {}

        InputViewerButton() = default;
        ~InputViewerButton() = default;
        void onReset();
        void onCalc();
        void setParams(EType, u16);

    private:
        EType m_type;
        u16 m_button;

        void hold();
        void release();
};

}
