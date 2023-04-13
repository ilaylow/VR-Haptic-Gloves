#include "controller_device.h"

// 3x3 or 3x4 matrix
template < class T >
vr::HmdQuaternion_t HmdQuaternion_FromMatrix(const T& matrix)
{
    vr::HmdQuaternion_t q{};

    q.w = sqrt(fmax(0, 1 + matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2])) / 2;
    q.x = sqrt(fmax(0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])) / 2;
    q.y = sqrt(fmax(0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2])) / 2;
    q.z = sqrt(fmax(0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2])) / 2;

    q.x = copysign(q.x, matrix.m[2][1] - matrix.m[1][2]);
    q.y = copysign(q.y, matrix.m[0][2] - matrix.m[2][0]);
    q.z = copysign(q.z, matrix.m[1][0] - matrix.m[0][1]);

    return q;
}

ControllerDevice::ControllerDevice(vr::ETrackedControllerRole role) : role_(role), device_id_(vr::k_unTrackedDeviceIndexInvalid), work(io), serial(io), timer(io) {
    serial.open("COM5");
    serial.set_option(boost::asio::serial_port_base::baud_rate(9600));
};

vr::EVRInitError ControllerDevice::Activate(uint32_t unObjectId) {
    vr::VRDriverLog()->Log("ControllerDevice::Activate");

    const vr::PropertyContainerHandle_t container = vr::VRProperties()->TrackedDeviceToPropertyContainer(unObjectId);
    vr::VRProperties()->SetStringProperty(container, vr::Prop_ModelNumber_String, "test_model_number");
    vr::VRProperties()->SetInt32Property(container, vr::Prop_ControllerRoleHint_Int32, role_);

    device_id_ = unObjectId;
    return vr::VRInitError_None;
}

void ControllerDevice::Deactivate() {
}

void ControllerDevice::EnterStandby() {
}

void* ControllerDevice::GetComponent(const char* pchComponentNameAndVersion) {
    return nullptr;
}

void ControllerDevice::DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) {
    if (unResponseBufferSize >= 1)
        pchResponseBuffer[0] = 0;
}

vr::DriverPose_t ControllerDevice::GetPose() {
    std::string data;

    try {
        boost::asio::read_until(serial, boost::asio::dynamic_buffer(data), '\n');
        vr::VRDriverLog()->Log("Read Success!");
        vr::VRDriverLog()->Log(data.c_str());
    }
    catch (const boost::system::system_error& e) {
        vr::VRDriverLog()->Log(("Error reading serial: " + std::string(e.what())).c_str());
    }

    vr::DriverPose_t pose = { 0 };

    pose.poseIsValid = true;
    pose.result = vr::TrackingResult_Running_OK;
    pose.deviceIsConnected = true;

    pose.qWorldFromDriverRotation.w = 1.f;
    pose.qDriverFromHeadRotation.w = 1.f;

    vr::TrackedDevicePose_t hmd_pose{};
    vr::VRServerDriverHost()->GetRawTrackedDevicePoses(0.f, &hmd_pose, 1);
       
    const vr::HmdQuaternion_t hmd_orientation = HmdQuaternion_FromMatrix(hmd_pose.mDeviceToAbsoluteTracking);
    pose.qRotation = hmd_orientation;

    pose.vecPosition[0] = role_ == vr::TrackedControllerRole_LeftHand
        ? hmd_pose.mDeviceToAbsoluteTracking.m[0][3] - 0.2f
        : hmd_pose.mDeviceToAbsoluteTracking.m[0][3] + 0.2f; // set the x position offset based on whether we are a left or right hand (so the controllers don't appear on top of eachother). This will
    // make it look like two controllers appearing side by side, 0.4 metres apart.

    pose.vecPosition[1] = hmd_pose.mDeviceToAbsoluteTracking.m[1][3];		// no vertical offset
    pose.vecPosition[2] = hmd_pose.mDeviceToAbsoluteTracking.m[2][3] - 0.5f; // make the controller appear half a metre in front of the hmd (-z is forward in space)

    return pose;
}

void ControllerDevice::RunFrame() {
    vr::VRServerDriverHost()->TrackedDevicePoseUpdated(device_id_, GetPose(), sizeof(vr::DriverPose_t));

    vr::VRDriverLog()->Log(this->curr_angle.c_str());
}

