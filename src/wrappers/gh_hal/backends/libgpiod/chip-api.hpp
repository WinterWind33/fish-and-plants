// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <gpiod.hpp>

// C++ STL
#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include <cstdint>

namespace gh_hal::backends::libgpiod_impl {

    using NativeChipType = ::gpiod::chip;
    using NativeLineRequestType = ::gpiod::line_request;
    using NativeLineDirectionType = ::gpiod::line::direction;
    using NativeLineOffsetType = std::uint32_t;

    std::unique_ptr<NativeChipType> openChip(std::filesystem::path chipPath);

    NativeLineRequestType requestLines(
        NativeChipType& chip,
        std::string consumer,
        std::vector<NativeLineOffsetType> offsets,
        const NativeLineDirectionType direction
    );

} // namespace gh_hal::backends::libgpiod_impl
