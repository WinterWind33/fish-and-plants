// Copyright (c) 2023 Andrea Ballestrazzi

#include <automatic-watering/hardware-controllers/daily-cycle-aws-hardware-controller.hpp>

// HAL
#include <gh_hal/hal-board-pin.hpp>

namespace rpi_gc::automatic_watering {

    DailyCycleAWSHardwareController::DailyCycleAWSHardwareController(
        const digital_output_id waterValvePinId, const digital_output_id waterPumpValvePinId) noexcept :
        m_waterValveDigitalOut{std::make_unique<gh_hal::HALBoardPin>(waterValvePinId)},
        m_waterPumpDigitalOut{std::make_unique<gh_hal::HALBoardPin>(waterPumpValvePinId)} {}

    void DailyCycleAWSHardwareController::setWaterValveDigitalOutputID(const digital_output_id pinID) noexcept {
        change_digital_out(m_waterValveDigitalOut, pinID);
    }

    void DailyCycleAWSHardwareController::setWaterPumpDigitalOutputID(const digital_output_id pinID) noexcept {
        change_digital_out(m_waterPumpDigitalOut, pinID);
    }

    void DailyCycleAWSHardwareController::change_digital_out(
        std::unique_ptr<digital_output_type>& pin, const digital_output_id newPinID) noexcept {
        pin->turnOff();
        pin = std::make_unique<gh_hal::HALBoardPin>(newPinID);
    }

} // namespace rpi_gc::automatic_watering
