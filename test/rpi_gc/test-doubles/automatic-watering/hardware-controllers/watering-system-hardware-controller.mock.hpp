// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef WATERING_SYSTEM_HARDWARE_CONTROLLER_MOCK_HPP
#define WATERING_SYSTEM_HARDWARE_CONTROLLER_MOCK_HPP

#include <automatic-watering/hardware-controllers/watering-system-hardware-controller.hpp>

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

namespace rpi_gc::automatic_watering::mocks {

    class WateringSystemHardwareControllerMock : public WateringSystemHardwareController {
    public:
        MOCK_METHOD(digital_output_type*, getWaterValveDigitalOut, (), (noexcept, override));
        MOCK_METHOD(digital_output_type*, getWaterPumpDigitalOut, (), (noexcept, override));
    };

} // namespace rpi_gc::automatic_watering::mocks

#endif // !WATERING_SYSTEM_HARDWARE_CONTROLLER_MOCK_HPP
