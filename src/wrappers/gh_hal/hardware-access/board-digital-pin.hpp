// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// C++ STL
#include <cstdint>
#include <ostream>

namespace gh_hal::hardware_access {

    enum class DigitalPinRequestDirection {
        Input,
        Output
    };

    struct BoardDigitalPin {
        using offset_type = std::uint32_t;

        virtual ~BoardDigitalPin() noexcept = default;

        virtual DigitalPinRequestDirection getDirection() const noexcept = 0;

        virtual void activate() noexcept = 0;
        virtual void deactivate() noexcept = 0;

        virtual void printStatus(std::ostream& ost) const noexcept = 0;
    };

    inline std::ostream& operator<<(std::ostream& ost, const BoardDigitalPin& digPin) noexcept {
        digPin.printStatus(ost);
        return ost;
    }

} // namespace gh_hal::hardware_access
