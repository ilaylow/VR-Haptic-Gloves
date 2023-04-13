#pragma once

#include "openvr_driver.h"
#include <boost/asio.hpp>

class ControllerDevice : public vr::ITrackedDeviceServerDriver {
public:
    ControllerDevice(vr::ETrackedControllerRole role);

    // Inherited via ITrackedDeviceServerDriver
    virtual vr::EVRInitError Activate(uint32_t unObjectId) override;
    virtual void Deactivate() override;
    virtual void EnterStandby() override;
    virtual void* GetComponent(const char* pchComponentNameAndVersion) override;
    virtual void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) override;
    virtual vr::DriverPose_t GetPose() override;

    void RunFrame();

    boost::asio::io_service io;
    boost::asio::io_context::work work;
    boost::asio::serial_port serial;
    boost::asio::steady_timer timer;
    std::string curr_angle;

private:
    vr::ETrackedControllerRole role_;
    vr::TrackedDeviceIndex_t device_id_;
};