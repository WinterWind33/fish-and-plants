// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef DAILY_CYCLE_AWS_HARDWARE_CONTROLLER_HPP
#define DAILY_CYCLE_AWS_HARDWARE_CONTROLLER_HPP

#include <automatic-watering/hardware-controllers/watering-system-hardware-controller.hpp>

// HAL
#include <gh_hal/hardware-access/board-chip.hpp>
#include <gh_hal/hardware-access/board-digital-pin.hpp>

// C++ STL
#include <memory>
#include <functional>
#include <tuple>

namespace rpi_gc::automatic_watering {

    class DailyCycleAWSHardwareController final : public WateringSystemHardwareController {
    public:
        using digital_output_id = gh_hal::hardware_access::BoardDigitalPin::offset_type;
        using chip_reference = std::reference_wrapper<gh_hal::hardware_access::BoardChip>;

        explicit DailyCycleAWSHardwareController(
            chip_reference chipRef, const digital_output_id waterValvePinId, const digital_output_id waterPumpValvePinId) noexcept;

        inline digital_output_type* getWaterValveDigitalOut() noexcept override {
            return std::get<1>(m_waterValveDigitalOut).get();
        }

        inline digital_output_type* getWaterPumpDigitalOut() noexcept override {
            return std::get<1>(m_waterPumpDigitalOut).get();
        }

        void setWaterValveDigitalOutputID(const digital_output_id id) noexcept;
        void setWaterPumpDigitalOutputID(const digital_output_id id) noexcept;

    private:
        chip_reference m_chipRef;
        std::pair<digital_output_id, std::unique_ptr<digital_output_type>> m_waterValveDigitalOut{};
        std::pair<digital_output_id, std::unique_ptr<digital_output_type>> m_waterPumpDigitalOut{};

        void change_digital_out(std::pair<digital_output_id, std::unique_ptr<digital_output_type>>& oldPin, const digital_output_id newPinID) noexcept;
        void request_line_pool() noexcept;
    };

} // namespace rpi_gc::automatic_watering

#endif // !DAILY_CYCLE_AWS_HARDWARE_CONTROLLER_HPP
