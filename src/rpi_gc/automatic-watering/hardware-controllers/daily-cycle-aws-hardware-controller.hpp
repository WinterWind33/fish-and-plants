// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef DAILY_CYCLE_AWS_HARDWARE_CONTROLLER_HPP
#define DAILY_CYCLE_AWS_HARDWARE_CONTROLLER_HPP

#include <automatic-watering/hardware-controllers/watering-system-hardware-controller.hpp>

// HAL
#include <gh_hal/hal-definitions.hpp>

// C++ STL
#include <memory>

namespace rpi_gc::automatic_watering {

    class DailyCycleAWSHardwareController : public WateringSystemHardwareController {
    public:
        using digital_output_id = gh_hal::PinID;

        explicit DailyCycleAWSHardwareController(
            const digital_output_id waterValvePinId, const digital_output_id waterPumpValvePinId) noexcept;

        inline digital_output_type* getWaterValveDigitalOut() noexcept override {
            return m_waterValveDigitalOut.get();
        }

        inline digital_output_type* getWaterPumpDigitalOut() noexcept override {
            return m_waterPumpDigitalOut.get();
        }

    private:
        std::unique_ptr<digital_output_type> m_waterValveDigitalOut{};
        std::unique_ptr<digital_output_type> m_waterPumpDigitalOut{};
    };

} // namespace rpi_gc::automatic_watering

#endif // !DAILY_CYCLE_AWS_HARDWARE_CONTROLLER_HPP
