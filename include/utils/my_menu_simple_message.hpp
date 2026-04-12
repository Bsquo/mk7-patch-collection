#pragma once

#include "UI/ControlAnimator.hpp"
#include "UI/MenuSimpleMessage.hpp"

namespace mod {
namespace utils {

class MyMenuSimpleMessage: public UI::MenuSimpleMessage {
    public:
        struct AnimationDefine: public UI::MenuSimpleMessage::AnimationDefine {};

        struct CreateArg: public UI::MenuSimpleMessage::CreateArg {
            void _0x0() {}
            void _0x8() {}
            const UI::ControlAnimator::AnimationDefine *getAnimationDefine() const { return &m_animation_define; }
        };

        void _0x2C() {}
        void _0x38() {}
        void _0x44() {}

        MyMenuSimpleMessage() = default;
        ~MyMenuSimpleMessage() = default;
        void setMessage(const UI::MessageString &);
};

}
}