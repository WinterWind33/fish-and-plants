// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef WATERING_SYSTEM_TIME_PROVIDER_HPP
#define WATERING_SYSTEM_TIME_PROVIDER_HPP

// C++ STL
#include <chrono>

namespace rpi_gc::automatic_watering {

    class WateringSystemTimeProvider {
    public:
        using time_unit = std::chrono::milliseconds;

        virtual ~WateringSystemTimeProvider() noexcept = default;

        virtual time_unit getWateringSystemActivationDuration() const noexcept = 0;
        virtual time_unit getWateringSystemDeactivationDuration() const noexcept = 0;
        virtual time_unit getPumpValveDeactivationTimeSeparation() const noexcept = 0;

        virtual void setWateringSystemActivationDuration(const time_unit duration) noexcept = 0;
        virtual void setWateringSystemDeactivationDuration(const time_unit duration) noexcept = 0;
        virtual void setPumpValveDeactivationTimeSeparation(const time_unit duration) noexcept = 0;
    };

} // namespace rpi_gc::automatic_watering

#endif // !WATERING_SYSTEM_TIME_PROVIDER_HPP
