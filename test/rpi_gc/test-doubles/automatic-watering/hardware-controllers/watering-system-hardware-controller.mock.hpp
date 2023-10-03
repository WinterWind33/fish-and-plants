// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <automatic-watering/hardware-controllers/watering-system-hardware-controller.hpp>

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

namespace rpi_gc::automatic_watering::mocks {

class WateringSystemHardwareControllerMock : public WateringSystemHardwareController {
public:
    MOCK_METHOD(digital_output_type*, getWaterValveDigitalOut, (), (noexcept, override));
    MOCK_METHOD(digital_output_type*, getWaterPumpDigitalOut, (), (noexcept, override));
    MOCK_METHOD(void, setWaterValveDigitalOutputID,
                (const digital_output_type::offset_type, const activation_state),
                (noexcept, override));
    MOCK_METHOD(void, setWaterPumpDigitalOutputID,
                (const digital_output_type::offset_type, const activation_state),
                (noexcept, override));
};

} // namespace rpi_gc::automatic_watering::mocks
