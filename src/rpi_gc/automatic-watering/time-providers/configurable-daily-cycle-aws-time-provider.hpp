// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef CONFIGURABLE_DAILY_CYCLE_AWS_TIME_PROVIDER_HPP
#define CONFIGURABLE_DAILY_CYCLE_AWS_TIME_PROVIDER_HPP

#include <automatic-watering/time-providers/watering-system-time-provider.hpp>

// C++ STL
#include <atomic>
#include <cstdint>

namespace rpi_gc::automatic_watering {

class ConfigurableDailyCycleAWSTimeProvider : public WateringSystemTimeProvider {
public:
    using rep_type = time_unit::rep;

    ConfigurableDailyCycleAWSTimeProvider(const time_unit initialActivationTime,
                                          const time_unit initialDeactivationTime,
                                          const time_unit initialPumpValveWaitTime) noexcept;

    [[nodiscard]] time_unit getWateringSystemActivationDuration() const noexcept override;

    [[nodiscard]] time_unit getWateringSystemDeactivationDuration() const noexcept override;

    [[nodiscard]] time_unit getPumpValveDeactivationTimeSeparation() const noexcept override;

    void setActivationTimeTicks(const rep_type ticks) noexcept;
    void setDeactivationTimeTicks(const rep_type ticks) noexcept;
    void setPumpValveWaitTimeTicks(const rep_type ticks) noexcept;

    void setWateringSystemActivationDuration(const time_unit duration) noexcept override;
    void setWateringSystemDeactivationDuration(const time_unit duration) noexcept override;
    void setPumpValveDeactivationTimeSeparation(const time_unit duration) noexcept override;

private:
    std::atomic<rep_type> m_activationTimeTicks{};
    std::atomic<rep_type> m_deactivationTimeTicks{};
    std::atomic<rep_type> m_pumpValveWaitTimeTicks{};
};

} // namespace rpi_gc::automatic_watering

#endif // !CONFIGURABLE_DAILY_CYCLE_AWS_TIME_PROVIDER_HPP
