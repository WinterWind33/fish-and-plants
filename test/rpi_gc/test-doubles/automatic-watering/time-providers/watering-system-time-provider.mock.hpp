// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef WATERING_SYSTEM_TIME_PROVIDER_MOCK_HPP
#define WATERING_SYSTEM_TIME_PROVIDER_MOCK_HPP

#include <automatic-watering/time-providers/watering-system-time-provider.hpp>

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

namespace rpi_gc::automatic_watering::mocks {

    class WateringSystemTimeProviderMock : public WateringSystemTimeProvider {
    public:
        MOCK_METHOD(time_unit, getWateringSystemActivationDuration, (), (const, noexcept, override));
        MOCK_METHOD(time_unit, getWateringSystemDeactivationDuration, (), (const, noexcept, override));
        MOCK_METHOD(time_unit, getPumpValveDeactivationTimeSeparation, (), (const, noexcept, override));
    };

} // namespace rpi_gc::automatic_watering::mocks

#endif // !WATERING_SYSTEM_TIME_PROVIDER_MOCK_HPP
