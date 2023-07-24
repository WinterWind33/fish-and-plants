// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef DAILY_CYCLE_AWS_TIME_PROVIDER_HPP
#define DAILY_CYCLE_AWS_TIME_PROVIDER_HPP

#include <automatic-watering/time-providers/watering-system-time-provider.hpp>

namespace rpi_gc::automatic_watering {

    class DailyCycleAWSTimeProvider final : public WateringSystemTimeProvider {
    public:

        [[nodiscard]] constexpr time_unit getWateringSystemActivationDuration() const noexcept override {
            return m_activationTime;
        }

        [[nodiscard]] constexpr time_unit getWateringSystemDeactivationDuration() const noexcept override {
            return m_deactivationTime;
        }

        [[nodiscard]] constexpr time_unit getPumpValveDeactivationTimeSeparation() const noexcept override {
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
        constexpr static std::size_t DEFAULT_ACTIVATION_TIME = 6000;
        constexpr static std::size_t DEFAULT_DEACTIVATION_TIME = 600'000;
        constexpr static std::size_t DEFAULT_PUMP_VALVE_DEACTIVATION_TIME_SEPARATION = 600;

        time_unit m_activationTime{DEFAULT_ACTIVATION_TIME};
        time_unit m_deactivationTime{DEFAULT_DEACTIVATION_TIME};
        time_unit m_pumpValveDeactivationTimeSeparation{DEFAULT_PUMP_VALVE_DEACTIVATION_TIME_SEPARATION};
    };

} // namespace automatic_watering

#endif // !DAILY_CYCLE_AWS_TIME_PROVIDER_HPP
