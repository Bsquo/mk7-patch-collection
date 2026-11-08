#include "mod_common.hpp"
#include "examples/class_lr_select/class_lr_select.hpp"
#include "System/RootSystem.hpp"
#include "Camera/KartCameraDirector.hpp"
#include "Demo/DemoCameraDirector.hpp"

void setKartCameraState(Camera::KartCameraDirector::EMode mode) {
    System::g_root_system->get_kart_camera_director()->_setMode(mode);
}

HOOK void enableUnusedCamera_Goal() {
    if (g_enable_unused_demo_cameras) {
        setKartCameraState(Camera::KartCameraDirector::EMode::DEMO);
    }
}

HOOK void enableUnusedCamera_Start() {
    if (g_enable_unused_demo_cameras) {
        Util::TStateObserver<Demo::DemoCameraDirector> *state_observer = &System::g_root_system->get_demo_camera_director()->m_state_observer;

        // We hook in ``, which also runs when performing the course preview.
        // So, in order to prevent overriding the course cameras with the start camera,
        // we first check if DemoCameraDirector is currently not playing any demo cameras
        // (i.e. not in its "WAIT" state) before allowing it to play the start camera.
        if (state_observer->m_state_current == static_cast<u8>(Demo::DemoCameraDirector::EState::WAIT)) {
            setKartCameraState(Camera::KartCameraDirector::EMode::DEMO);
            state_observer->changeState(static_cast<u8>(Demo::DemoCameraDirector::EState::START));
            System::g_root_system->get_kart_camera_director()->m_is_start_demo = true;
        }
    }
}