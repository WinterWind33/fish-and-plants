// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_hal/backends/libgpiod/chip-api.hpp>

namespace gh_hal::backends::libgpiod_impl {

    std::unique_ptr<gpiod::chip> openChip(std::filesystem::path chipPath) {
        return std::make_unique<gpiod::chip>(chipPath);
    }

} // namespace gh_hal::backends::libgpiod_impl
