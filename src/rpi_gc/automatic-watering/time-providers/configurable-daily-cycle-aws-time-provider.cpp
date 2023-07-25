// Copyright (c) Andrea Ballestrazzi
#include <automatic-watering/time-providers/configurable-daily-cycle-aws-time-provider.hpp>

namespace rpi_gc::automatic_watering {

ConfigurableDailyCycleAWSTimeProvider::ConfigurableDailyCycleAWSTimeProvider(
    const time_unit initialActivationTime, const time_unit initialDeactivationTime,
    const time_unit initialPumpValveWaitTime) noexcept
    : m_activationTimeTicks{initialActivationTime.count()},
      m_deactivationTimeTicks{initialDeactivationTime.count()},
      m_pumpValveWaitTimeTicks{initialPumpValveWaitTime.count()} {}

auto ConfigurableDailyCycleAWSTimeProvider::getWateringSystemActivationDuration() const noexcept
    -> time_unit {
    return time_unit{m_activationTimeTicks.load()};
}

auto ConfigurableDailyCycleAWSTimeProvider::getWateringSystemDeactivationDuration() const noexcept
    -> time_unit {
    return time_unit{m_deactivationTimeTicks.load()};
}

auto ConfigurableDailyCycleAWSTimeProvider::getPumpValveDeactivationTimeSeparation() const noexcept
    -> time_unit {
    return time_unit{m_pumpValveWaitTimeTicks.load()};
}

void ConfigurableDailyCycleAWSTimeProvider::setActivationTimeTicks(const rep_type ticks) noexcept {
    m_activationTimeTicks.store(ticks);
}

void ConfigurableDailyCycleAWSTimeProvider::setDeactivationTimeTicks(
    const rep_type ticks) noexcept {
    m_deactivationTimeTicks.store(ticks);
}

void ConfigurableDailyCycleAWSTimeProvider::setPumpValveWaitTimeTicks(
    const rep_type ticks) noexcept {
    m_pumpValveWaitTimeTicks.store(ticks);
}

void ConfigurableDailyCycleAWSTimeProvider::setWateringSystemActivationDuration(
    const time_unit duration) noexcept {
    setActivationTimeTicks(duration.count());
}

void ConfigurableDailyCycleAWSTimeProvider::setWateringSystemDeactivationDuration(
    const time_unit duration) noexcept {
    setDeactivationTimeTicks(duration.count());
}

void ConfigurableDailyCycleAWSTimeProvider::setPumpValveDeactivationTimeSeparation(
    const time_unit duration) noexcept {
    setPumpValveWaitTimeTicks(duration.count());
}

} // namespace rpi_gc::automatic_watering
