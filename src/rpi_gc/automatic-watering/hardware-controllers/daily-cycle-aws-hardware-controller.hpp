// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <automatic-watering/hardware-controllers/watering-system-hardware-controller.hpp>

// HAL
#include <gh_hal/hardware-access/board-chip.hpp>
#include <gh_hal/hardware-access/board-digital-pin.hpp>

// C++ STL
#include <functional>
#include <memory>
#include <mutex>
#include <tuple>

namespace rpi_gc::automatic_watering {

//!!
//! \brief Represents a hardware controller for a watering system that uses a daily cycle mode.
//!
class DailyCycleAWSHardwareController final : public WateringSystemHardwareController {
public:
    using digital_output_id = gh_hal::hardware_access::BoardDigitalPin::offset_type;
    using chip_reference = std::reference_wrapper<gh_hal::hardware_access::BoardChip>;
    using mutex_reference = std::reference_wrapper<std::mutex>;

    //!!
    //! \brief Construct a new object with the given parameters.
    //!
    //! \param mutex A reference to the mutex that will be used to synchronize the access to the
    //!  hardware.
    //! \param chipRef A reference to the chip that will be used to create the hardware lines.
    //! \param waterValvePinId The ID of the digital output that will be used to control the water
    //!  valve.
    //! \param waterPumpValvePinId The ID of the digital output that will be used to control the
    //!  water pump.
    //! \param valveActivationState The activation state of the water valve.
    //! \param pumpActivationState The activation state of the water pump.
    //!
    explicit DailyCycleAWSHardwareController(
        mutex_reference mutex, chip_reference chipRef, const digital_output_id waterValvePinId,
        const digital_output_id waterPumpValvePinId,
        const activation_state valveActivationState = activation_state::ActiveLow,
        const activation_state pumpActivationState = activation_state::ActiveLow) noexcept;

    inline digital_output_type* getWaterValveDigitalOut() noexcept override {
        return std::get<1>(m_waterValveDigitalOut).get();
    }

    inline digital_output_type* getWaterPumpDigitalOut() noexcept override {
        return std::get<1>(m_waterPumpDigitalOut).get();
    }

    //!!
    //! \brief Set the water valve digital output with the given activation state.
    //!
    //! \param id The ID of the digital output.
    //! \param newActivationState The new activation state of the digital output.
    void setWaterValveDigitalOutputID(
        const digital_output_id id,
        const activation_state newActivationState = activation_state::ActiveLow) noexcept override;

    //!!
    //! \brief Set the water pump digital output with the given activation state.
    //!
    //! \param id The ID of the digital output.
    //! \param newActivationState The new activation state of the digital output.
    //!
    void setWaterPumpDigitalOutputID(
        const digital_output_id id,
        const activation_state newActivationState = activation_state::ActiveLow) noexcept override;

private:
    mutex_reference m_mutex;
    chip_reference m_chipRef;
    std::pair<digital_output_id, std::unique_ptr<digital_output_type>> m_waterValveDigitalOut{};
    std::pair<digital_output_id, std::unique_ptr<digital_output_type>> m_waterPumpDigitalOut{};

    void change_digital_out(
        std::pair<digital_output_id, std::unique_ptr<digital_output_type>>& oldPin,
        const digital_output_id newPinID, const activation_state newActivationState) noexcept;
};

} // namespace rpi_gc::automatic_watering
