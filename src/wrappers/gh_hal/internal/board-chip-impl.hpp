// Copyright (c) 2023 Andrea ballestrazzi
#pragma once

#include <gh_hal/hardware-access/board-chip.hpp>
#include <gh_hal/hardware-access/board-digital-pin.hpp>

#include <gh_hal/internal/line-request.hpp>

#ifdef USE_LIBGPIOD
#include <gh_hal/backends/libgpiod/chip-api.hpp>
#else
#include <gh_hal/backends/simulated/simulated-chip.hpp>
#endif // USE_LIBGPIOD

// C++ STL
#include <vector>

namespace gh_hal::internal {

namespace details {
#ifdef USE_LIBGPIOD
using BackendChipType = gpiod::chip;
#else
using BackendChipType = backends::simulated::SimulatedChip;
#endif // USE_LIBGPIOD
} // namespace details

class BoardChipImpl final : public gh_hal::hardware_access::BoardChip {
public:
    using chip_unique_ptr = std::unique_ptr<details::BackendChipType>;
    using offsets_vector = std::vector<hardware_access::BoardDigitalPin::offset_type>;

    explicit BoardChipImpl(const std::filesystem::path& chipPath);

    // Line requests
    std::unique_ptr<hardware_access::BoardDigitalPin> requestDigitalPin(
        std::string consumer, hardware_access::BoardDigitalPin::offset_type offset,
        const hardware_access::DigitalPinRequestDirection direction,
        const hardware_access::DigitalOutPinActivationState activationState) noexcept override;

    std::vector<std::unique_ptr<hardware_access::BoardDigitalPin>> requestDigitalPinPool(
        std::string consumer, std::vector<hardware_access::BoardDigitalPin::offset_type> offset,
        const hardware_access::DigitalPinRequestDirection direction,
        const hardware_access::DigitalOutPinActivationState activationState) noexcept override;

    bool releaseRequest(
        std::vector<hardware_access::BoardDigitalPin::offset_type> offsets) noexcept override;

    explicit operator bool() const noexcept;

private:
    chip_unique_ptr m_chipPtr{};
    std::vector<std::pair<offsets_vector, LineRequest>> m_lineRequests{};
};

} // namespace gh_hal::internal
