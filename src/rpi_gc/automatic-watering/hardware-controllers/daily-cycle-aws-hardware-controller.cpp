// Copyright (c) 2023 Andrea Ballestrazzi

#include <automatic-watering/hardware-controllers/daily-cycle-aws-hardware-controller.hpp>

// C++ STL
#include <cassert>
#include <string_view>

namespace rpi_gc::automatic_watering {

namespace details {
constexpr std::string_view HARDWARE_CONTROLLER_CONSUMER{"FeP_Daily_Cycle_AWS_Hardware_Controller"};
} // namespace details

DailyCycleAWSHardwareController::DailyCycleAWSHardwareController(
    mutex_reference mutex, chip_reference chipRef, const digital_output_id waterValvePinId,
    const digital_output_id waterPumpValvePinId) noexcept
    : m_mutex{mutex},
      m_chipRef{chipRef} {
    m_waterValveDigitalOut = std::make_pair(
        waterValvePinId, m_chipRef.get().requestDigitalPin(
                             std::string{details::HARDWARE_CONTROLLER_CONSUMER}, waterValvePinId,
                             gh_hal::hardware_access::DigitalPinRequestDirection::Output));

    m_waterPumpDigitalOut =
        std::make_pair(waterPumpValvePinId,
                       m_chipRef.get().requestDigitalPin(
                           std::string{details::HARDWARE_CONTROLLER_CONSUMER}, waterPumpValvePinId,
                           gh_hal::hardware_access::DigitalPinRequestDirection::Output));
}

void DailyCycleAWSHardwareController::setWaterValveDigitalOutputID(
    const digital_output_id pinID) noexcept {
    std::lock_guard<std::mutex> lock{m_mutex.get()};
    change_digital_out(m_waterValveDigitalOut, pinID);
}

void DailyCycleAWSHardwareController::setWaterPumpDigitalOutputID(
    const digital_output_id pinID) noexcept {
    std::lock_guard<std::mutex> lock{m_mutex.get()};
    change_digital_out(m_waterPumpDigitalOut, pinID);
}

void DailyCycleAWSHardwareController::change_digital_out(
    std::pair<digital_output_id, std::unique_ptr<digital_output_type>>& oldPin,
    const digital_output_id newPinID) noexcept {
    assert(std::get<1>(oldPin) != nullptr);

    std::get<1>(oldPin)->deactivate();

    // We firstly need to release the previous request.
    const bool bRes{m_chipRef.get().releaseRequest({std::get<0>(oldPin)})};
    assert(bRes);

    // Then we can create a new one.
    oldPin =
        std::make_pair(newPinID, m_chipRef.get().requestDigitalPin(
                                     std::string{details::HARDWARE_CONTROLLER_CONSUMER}, newPinID,
                                     gh_hal::hardware_access::DigitalPinRequestDirection::Output));
}

} // namespace rpi_gc::automatic_watering
