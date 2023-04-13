#include "device_provider.h"

vr::EVRInitError DeviceProvider::Init(vr::IVRDriverContext* pDriverContext) {
    VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);

    vr::VRDriverLog()->Log("Adding in devices to runtime...");
    right_hand_device_ = std::make_unique<ControllerDevice>(vr::TrackedControllerRole_RightHand);
    vr::VRServerDriverHost()->TrackedDeviceAdded("<right_serial_number>",
        vr::TrackedDeviceClass_Controller,
        right_hand_device_.get());

    return vr::VRInitError_None;
}

void DeviceProvider::Cleanup() {
    VR_CLEANUP_SERVER_DRIVER_CONTEXT();
}

const char* const* DeviceProvider::GetInterfaceVersions() {
    return vr::k_InterfaceVersions;
}

void DeviceProvider::RunFrame() {
    if (right_hand_device_ != nullptr) {
        right_hand_device_->RunFrame();
    }
}

bool DeviceProvider::ShouldBlockStandbyMode() {
    return false;
}

void DeviceProvider::EnterStandby() {

}

void DeviceProvider::LeaveStandby() {

}