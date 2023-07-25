// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// C++ STL
#include <cstdint>
#include <limits>

namespace gh_hal::backends::simulated {

//!!
//! \brief Represents a simulated digital board pin with simulated (empty)
//!  activation and deactivation. The board pin must be initialized with an
//!  offset that will identify the pin.
class DigitalBoardPin final {
public:
    using offset_type = std::uint32_t;

    constexpr DigitalBoardPin() noexcept = default;

    constexpr DigitalBoardPin(const offset_type offsetValue) noexcept : m_pinOffset{offsetValue} {}

    constexpr void activate() const noexcept {}
    constexpr void deactivate() const noexcept {}

    constexpr offset_type getOffsetValue() const noexcept {
        return m_pinOffset;
    }

    constexpr explicit operator bool() const noexcept {
        return m_pinOffset != INVALID_OFFSET;
    }

private:
    constexpr static offset_type INVALID_OFFSET{std::numeric_limits<offset_type>::max()};

    offset_type m_pinOffset{INVALID_OFFSET};
};

} // namespace gh_hal::backends::simulated
