// Copyright (c) 2023 Andrea ballestrazzi
#pragma once

#include <gh_hal/hardware-access/board-chip.hpp>

#ifdef USE_LIBGPIOD
#include <gh_hal/backends/libgpiod/chip-api.hpp>
#else
#error "No other backend library apart from libgpiod"
#endif // USE_LIBGPIOD

namespace gh_hal::internal {

    class BoardChipImpl final : public gh_hal::hardware_access::BoardChip{
    public:
        using chip_unique_ptr = std::unique_ptr<gpiod::chip>;

        explicit BoardChipImpl(std::filesystem::path chipPath);

    private:
        chip_unique_ptr m_chipPtr{};
    };

} // namespace gh_hal::internal
