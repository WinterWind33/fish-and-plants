// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef DAILY_CYCLE_AWS_TIME_PROVIDER_HPP
#define DAILY_CYCLE_AWS_TIME_PROVIDER_HPP

#include <automatic-watering/time-providers/watering-system-time-provider.hpp>

namespace rpi_gc::automatic_watering {

    class DailyCycleAWSTimeProvider final : public WateringSystemTimeProvider {
    public:

        constexpr time_unit getWateringSystemActivationDuration() const noexcept final {
            return time_unit{6000};
        }

        constexpr time_unit getWateringSystemDeactivationDuration() const noexcept final {
            return time_unit{60'000};
        }

        constexpr time_unit getPumpValveDeactivationTimeSeparation() const noexcept final {
            return time_unit{600};
        }
    };

} // namespace automatic_watering

#endif // !DAILY_CYCLE_AWS_TIME_PROVIDER_HPP
