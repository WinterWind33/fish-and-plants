// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef EMERGENCY_STOPPABLE_SYSTEM_MOCK_HPP
#define EMERGENCY_STOPPABLE_SYSTEM_MOCK_HPP

#include <abort-system/emergency-stoppable-system.hpp>

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

namespace rpi_gc::abort_system::mocks {

class EmergencyStoppableSystemMock : public rpi_gc::abort_system::EmergencyStoppableSystem {
public:
    ~EmergencyStoppableSystemMock() noexcept override = default;

    MOCK_METHOD(void, emergencyAbort, (), (noexcept, override));
};

} // namespace rpi_gc::abort_system::mocks

#endif // !EMERGENCY_STOPPABLE_SYSTEM_MOCK_HPP
