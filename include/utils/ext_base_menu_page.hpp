#pragma once

#include "Sequence/BaseMenuPage.hpp"
#include "UI/ControlAnimator.hpp"
#include "UI/ControlDirector.hpp"
#include "UI/ControlInitializer.hpp"
#include "UI/ControlSight.hpp"

#include <prim/seadSafeString.hpp>

namespace mod {
namespace utils {

template <typename T>
T* setupControl(Sequence::BasePage *menu, const sead::SafeString &control_filename, const sead::SafeString &control_dataname) {
    UI::ControlInitializer *initializer = menu->m_control_initializer;
    UI::ControlDirector *director = initializer->m_control_director;

    typename T::CreateArg create_arg;
    // TODO: Hacky way to emit T::AnimationDefine's vtable
    new (&create_arg.m_animation_define) typename T::AnimationDefine;

    T *control = new T();
    control->initNode(director);
    director->appendNode(control);

    initializer->m_control = control;
    initializer->initCreateArg(&create_arg, control_filename, control_dataname);
    initializer->m_control_create_arg = &create_arg;
    initializer->endSetupControl(&create_arg);

    menu->m_controls.pushBack(control);

    return control;
}

template <typename T>
T* setupControlOutsideManipulatorArray(Sequence::BasePage *menu, const sead::SafeString &control_filename, const sead::SafeString &control_dataname) {
    UI::ControlInitializer *initializer = menu->m_control_initializer;
    UI::ControlDirector *director = initializer->m_control_director;
    menu->m_set_item_to_all_manipulators = true;

    typename T::CreateArg create_arg;
    // TODO: Hacky way to emit T::AnimationDefine's vtable
    new (&create_arg.m_animation_define) typename T::AnimationDefine;

    T *control = new T();
    control->initNode(director);
    director->appendNode(control);

    initializer->m_control = control;
    initializer->initCreateArg(&create_arg, control_filename, control_dataname);
    initializer->m_control_create_arg = &create_arg;
    initializer->endSetupControl(&create_arg);

    menu->m_set_item_to_all_manipulators = false;

    menu->m_controls_outside_manipulator_array.pushBack(control);

    return control;
}

}
}