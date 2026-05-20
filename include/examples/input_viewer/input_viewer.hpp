#pragma once

#include "forward.hpp"
#include "Object/Actor.hpp"
#include "UI/BaseMenuViewControl.hpp"

#include <controller/ctr/seadCtrController.h>
#include <math/seadVector.hpp>

namespace mod {

class InputViewerInput: public UI::BaseMenuViewControl {
    public:
        enum class EType : u32 {
            TYPE_BUTTON,
            TYPE_SHOULDER,
            TYPE_STICK
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

        InputViewerInput() = default;
        virtual ~InputViewerInput() = default;
        virtual void onReset();
        virtual void onCalc() = 0;
        virtual void setButton(u16) = 0;

        void hold();
        void release();
        sead::Vector2f getScale();
        void setScale(f32, f32);
        EType getType() const;
        void setType(EType);

    private:
        void setHoldState(bool);

        EType m_type;
};

class InputViewerButton: public InputViewerInput {
    public:
        ~InputViewerButton() = default;
        void onCalc();
        void setButton(u16);

    private:
        u16 m_button;
};

class InputViewerStick: public InputViewerInput {
    public:
        ~InputViewerStick() = default;
        void onReset();
        void onCalc();
        void setButton(u16);

    private:
        sead::Vector2s m_stick;
};

class InputViewer {
    public:
        enum InputButton : u32 {
            BUTTON_A,
            BUTTON_B,
            BUTTON_X,
            BUTTON_Y,
            BUTTON_L,
            BUTTON_R,
            BUTTON_STICK
        };

        InputViewer(Sequence::RacePage *);
        ~InputViewer();
        void applyInputPos();

    private:
        enum { NUM_INPUTS = 7 };

        static sead::Vector3f BUTTON_POSITIONS[NUM_INPUTS];
        InputViewerInput *m_inputs[NUM_INPUTS];
        Sequence::RacePage *m_race_page;

        inline void clear();
};

}
