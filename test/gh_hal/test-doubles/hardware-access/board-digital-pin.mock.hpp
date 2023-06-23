// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <gh_hal/hardware-access/board-digital-pin.hpp>

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

namespace gh_hal::hardware_access::mocks {

    class BoardDigitalPinMock : public BoardDigitalPin {
    public:

        MOCK_METHOD(DigitalPinRequestDirection, getDirection, (), (const, noexcept, final));
        MOCK_METHOD(void, activate, (), (noexcept, final));
        MOCK_METHOD(void, deactivate, (), (noexcept, final));
        MOCK_METHOD(void, printStatus, (std::ostream&), (const, noexcept, final));
        MOCK_METHOD(offset_type, getOffset, (), (const, noexcept, final));
    };

} // namespace gh_hal::hardware_access::mocks
