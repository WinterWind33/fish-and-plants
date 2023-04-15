// Copyright (c) 2023 Andrea ballestrazzi
#pragma once

#include <gh_hal/hardware-access/board-chip.hpp>

#ifdef USE_LIBGPIOD
#include <gh_hal/backends/libgpiod/chip-api.hpp>
#else
#include <gh_hal/backends/simulated/simulated-chip.hpp>
#endif // USE_LIBGPIOD

namespace gh_hal::internal {

    namespace details {
#ifdef USE_LIBGPIOD
        using BackendChipType = gpiod::chip;
#else
        using BackendChipType = backends::simulated::SimulatedChip;
#endif // USE_LIBGPIOD
    } // namespace details

    class BoardChipImpl final : public gh_hal::hardware_access::BoardChip{
    public:
        using chip_unique_ptr = std::unique_ptr<details::BackendChipType>;

        explicit BoardChipImpl(std::filesystem::path chipPath);

        explicit operator bool() const noexcept;

    private:
        chip_unique_ptr m_chipPtr{};
    };

} // namespace gh_hal::internal
