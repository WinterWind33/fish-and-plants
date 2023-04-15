// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <gpiod.hpp>

// C++ STL
#include <filesystem>
#include <memory>

namespace gh_hal::backends::libgpiod_impl {

    std::unique_ptr<gpiod::chip> openChip(std::filesystem::path chipPath);

} // namespace gh_hal::backends::libgpiod_impl
