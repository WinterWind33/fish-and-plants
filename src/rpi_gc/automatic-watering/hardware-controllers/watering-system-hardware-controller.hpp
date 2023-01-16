// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef WATERING_SYSTEM_HARDWARE_CONTROLLER
#define WATERING_SYSTEM_HARDWARE_CONTROLLER

// HAL
#include <gh_hal/hal-digital-output.hpp>

namespace rpi_gc::automatic_watering {

    class WateringSystemHardwareController {
    public:
        virtual ~WateringSystemHardwareController() noexcept = default;

        using digital_output_type = gh_hal::HALDigitalOutput;

        [[nodiscard]] virtual digital_output_type* getWaterValveDigitalOut() noexcept = 0;
        [[nodiscard]] virtual digital_output_type* getWaterPumpDigitalOut() noexcept = 0;
    };

} // namespace std::automatic_watering

#endif // !WATERING_SYSTEM_HARDWARE_CONTROLLER