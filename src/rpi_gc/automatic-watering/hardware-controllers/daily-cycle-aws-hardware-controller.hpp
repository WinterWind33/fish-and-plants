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

class DailyCycleAWSHardwareController final : public WateringSystemHardwareController {
public:
    using digital_output_id = gh_hal::hardware_access::BoardDigitalPin::offset_type;
    using chip_reference = std::reference_wrapper<gh_hal::hardware_access::BoardChip>;
    using mutex_reference = std::reference_wrapper<std::mutex>;

    explicit DailyCycleAWSHardwareController(mutex_reference mutex, chip_reference chipRef,
                                             const digital_output_id waterValvePinId,
                                             const digital_output_id waterPumpValvePinId) noexcept;

    inline digital_output_type* getWaterValveDigitalOut() noexcept override {
        return std::get<1>(m_waterValveDigitalOut).get();
    }

    inline digital_output_type* getWaterPumpDigitalOut() noexcept override {
        return std::get<1>(m_waterPumpDigitalOut).get();
    }

    void setWaterValveDigitalOutputID(const digital_output_id id) noexcept override;
    void setWaterPumpDigitalOutputID(const digital_output_id id) noexcept override;

private:
    mutex_reference m_mutex;
    chip_reference m_chipRef;
    std::pair<digital_output_id, std::unique_ptr<digital_output_type>> m_waterValveDigitalOut{};
    std::pair<digital_output_id, std::unique_ptr<digital_output_type>> m_waterPumpDigitalOut{};

    void change_digital_out(
        std::pair<digital_output_id, std::unique_ptr<digital_output_type>>& oldPin,
        const digital_output_id newPinID) noexcept;
};

} // namespace rpi_gc::automatic_watering
