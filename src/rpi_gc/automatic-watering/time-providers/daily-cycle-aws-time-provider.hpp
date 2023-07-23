// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef DAILY_CYCLE_AWS_TIME_PROVIDER_HPP
#define DAILY_CYCLE_AWS_TIME_PROVIDER_HPP

#include <automatic-watering/time-providers/watering-system-time-provider.hpp>

namespace rpi_gc::automatic_watering {

    class DailyCycleAWSTimeProvider final : public WateringSystemTimeProvider {
    public:

        constexpr time_unit getWateringSystemActivationDuration() const noexcept override {
            return m_activationTime;
        }

        constexpr time_unit getWateringSystemDeactivationDuration() const noexcept override {
            return m_deactivationTime;
        }

        constexpr time_unit getPumpValveDeactivationTimeSeparation() const noexcept override {
            return m_pumpValveDeactivationTimeSeparation;
        }

        [[nodiscard]] constexpr void setWateringSystemActivationDuration(const time_unit duration) noexcept override {
            m_activationTime = duration;
        }

        [[nodiscard]] constexpr void setWateringSystemDeactivationDuration(const time_unit duration) noexcept override {
            m_deactivationTime = duration;
        }

        [[nodiscard]] constexpr void setPumpValveDeactivationTimeSeparation(const time_unit duration) noexcept override {
            m_pumpValveDeactivationTimeSeparation = duration;
        }

    private:
        time_unit m_activationTime{6000};
        time_unit m_deactivationTime{600'000};
        time_unit m_pumpValveDeactivationTimeSeparation{600};
    };

} // namespace automatic_watering

#endif // !DAILY_CYCLE_AWS_TIME_PROVIDER_HPP
