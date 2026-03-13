#include "common.hpp"
#include "types.hpp"
#include "Debug/print.hpp"
#include "Kart/Vehicle/Vehicle_E32010.hpp"

HOOK void printKartInfo() {
    Kart::Vehicle *vehicle;
    READ_ARM_REG(r4, vehicle);

    Debug::PrintTextToBottomScreenChar(65, 10, "Speed: %.3f", vehicle->m_speed);
    Debug::PrintTextToBottomScreenChar(65, 20, "Pos X: %.3f", vehicle->m_pos->x);
    Debug::PrintTextToBottomScreenChar(65, 30, "Pos Y: %.3f", vehicle->m_pos->y);
    Debug::PrintTextToBottomScreenChar(65, 40, "Pos Z: %.3f", vehicle->m_pos->z);
    Debug::PrintTextToBottomScreenChar(65, 50, "Miniturbo charge: %.3f", vehicle->m_miniturbo_charge);
    Debug::PrintTextToBottomScreenChar(65, 60, "Speed ratio: %.3f", vehicle->m_speed_ratio);
    Debug::PrintTextToBottomScreenChar(65, 70, "Rotation X: %.3f", vehicle->m_rotation.x);
    Debug::PrintTextToBottomScreenChar(65, 80, "Rotation Y: %.3f", vehicle->m_rotation.y);
    Debug::PrintTextToBottomScreenChar(65, 90, "Rotation Z: %.3f", vehicle->m_rotation.z);
    Debug::PrintTextToBottomScreenChar(65, 100, "Boost duration: %d", vehicle->m_boost_duration);
    Debug::PrintTextToBottomScreenChar(65, 110, "Stick X (drift): %.3f", vehicle->m_stick.x);
    Debug::PrintTextToBottomScreenChar(65, 120, "Stick Y: %.3f", vehicle->m_stick.y);
    Debug::PrintTextToBottomScreenChar(65, 130, "Accelerating?: %s", vehicle->m_accelerating ? "TRUE" : "FALSE");
    Debug::PrintTextToBottomScreenChar(65, 140, "Braking?: %s", vehicle->m_breaking ? "TRUE" : "FALSE");
    Debug::PrintTextToBottomScreenChar(65, 150, "Drift input (held) ?: %s", vehicle->m_drifting_1 ? "TRUE" : "FALSE");
    Debug::PrintTextToBottomScreenChar(65, 160, "Drift input (pressed)?: %s", vehicle->m_drifting_2 ? "TRUE" : "FALSE");
}
