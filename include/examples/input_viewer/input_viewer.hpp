#pragma once

#include "UI/BaseMenuViewControl.hpp"

namespace mod {

class InputViewer: public UI::BaseMenuViewControl {
    public:
        struct AnimationDefine: public UI::MenuSimpleMessage::AnimationDefine {
            void defineAnimation();
        };

        struct CreateArg: public UI::MenuSimpleMessage::CreateArg {
            void _0x0() {}
            void _0x8() {}
            const UI::ControlAnimator::AnimationDefine *getAnimationDefine() const { return &m_animation_define; }
        };

        void _0x2C() {}
        void _0x38() {}
        void _0x44() {}

        InputViewer() = default;
        ~InputViewer() = default;
};

}
