// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef CONFIGURABLE_DAILY_CYCLE_AWS_TIME_PROVIDER_HPP
#define CONFIGURABLE_DAILY_CYCLE_AWS_TIME_PROVIDER_HPP

#include <automatic-watering/time-providers/watering-system-time-provider.hpp>

// C++ STL
#include <cstdint>
#include <atomic>

namespace rpi_gc::automatic_watering {

    class ConfigurableDailyCycleAWSTimeProvider : public WateringSystemTimeProvider {
    public:
        using rep_type = time_unit::rep;

        ConfigurableDailyCycleAWSTimeProvider(
            const time_unit initialActivationTime,
            const time_unit initialDeactivationTime,
            const time_unit initialPumpValveWaitTime) noexcept;


    private:
        std::atomic<rep_type> m_activationTimeTicks{};
        std::atomic<rep_type> m_deactivationTimeTicks{};
        std::atomic<rep_type> m_pumpValveWaitTimeTicks{};
    };

} // namespace rpi_gc::automatic_watering

#endif // !CONFIGURABLE_DAILY_CYCLE_AWS_TIME_PROVIDER_HPP
