// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// Third-party
#include <semver.hpp>

// C++ STL
#include <cstdint>
#include <string_view>
#include <optional>

namespace rpi_gc::version {

    constexpr std::uint16_t RPI_GC_VERSION_MAJOR{1};
    constexpr std::uint16_t RPI_GC_VERSION_MINOR{0};
    constexpr std::uint16_t RPI_GC_VERSION_PATCH{0};
    constexpr std::string_view RPI_GC_VERSION_RC_NAME{};

    namespace details{
        namespace prerelease {
            constexpr std::optional<std::uint16_t> RPI_GC_VERSION_RC_NUM{};
        } // namespace prerelease
    } // namespace details

    consteval semver::version getApplicationVersion() noexcept {
        if constexpr (details::prerelease::RPI_GC_VERSION_RC_NUM.has_value()) {
            return semver::version{
                RPI_GC_VERSION_MAJOR,
                RPI_GC_VERSION_MINOR,
                RPI_GC_VERSION_PATCH,
                semver::prerelease::rc,
                details::prerelease::RPI_GC_VERSION_RC_NUM};
        } else {

            return semver::version{
                RPI_GC_VERSION_MAJOR,
                RPI_GC_VERSION_MINOR,
                RPI_GC_VERSION_PATCH};
        }
    }

} // namespace rpi_gc::version
