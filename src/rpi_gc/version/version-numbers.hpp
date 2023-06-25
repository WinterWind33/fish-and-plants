// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// Third-party
#include <semver.hpp>

// C++ STL
#include <cstdint>
#include <string_view>

namespace rpi_gc::version {

    constexpr std::int32_t RPI_GC_VERSION_MAJOR{1};
    constexpr std::int32_t RPI_GC_VERSION_MINOR{0};
    constexpr std::int32_t RPI_GC_VERSION_PATCH{0};
    constexpr std::string_view RPI_GC_VERSION_RC_NAME{""};

} // namespace rpi_gc::version
