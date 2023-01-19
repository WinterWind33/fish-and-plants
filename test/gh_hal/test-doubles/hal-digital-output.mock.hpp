// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef HAL_DIGITAL_OUTPUT_MOCK_HPP
#define HAL_DIGITAL_OUTPUT_MOCK_HPP

#include <gh_hal/hal-digital-output.hpp>

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

namespace gh_hal::mocks {

    class HALDigitalOutputMock : public HALDigitalOutput {
    public:
        MOCK_METHOD(void, turnOn, (), (noexcept, override));
        MOCK_METHOD(void, turnOff, (), (noexcept, override));
    };

} // namespace gh_hal::mocks

#endif // !HAL_DIGITAL_OUTPUT_MOCK_HPP
