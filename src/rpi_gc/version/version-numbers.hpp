// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// Third-party
#include <semver.hpp>

// C++ STL
#include <cstdint>
#include <optional>

namespace rpi_gc::version {
namespace details {
constexpr std::uint16_t RPI_GC_VERSION_MAJOR{1};
constexpr std::uint16_t RPI_GC_VERSION_MINOR{1};
constexpr std::uint16_t RPI_GC_VERSION_PATCH{1};

namespace prerelease {
constexpr std::optional<std::uint16_t> RPI_GC_VERSION_RC_NUM{};
} // namespace prerelease
} // namespace details

//!!
//! \brief Retrieve rpi_gc application version.
//!
//! \return The version of the application.
[[nodiscard]] constexpr semver::version getApplicationVersion() noexcept {
    if constexpr (details::prerelease::RPI_GC_VERSION_RC_NUM.has_value()) {
        return semver::version{details::RPI_GC_VERSION_MAJOR, details::RPI_GC_VERSION_MINOR,
                               details::RPI_GC_VERSION_PATCH, semver::prerelease::rc,
                               details::prerelease::RPI_GC_VERSION_RC_NUM};
    } else {
        return semver::version{details::RPI_GC_VERSION_MAJOR, details::RPI_GC_VERSION_MINOR,
                               details::RPI_GC_VERSION_PATCH};
    }
}

} // namespace rpi_gc::version
