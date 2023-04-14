// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_hal/backends/libgpiod/miscellaneous.hpp>

#include <gpiod.hpp>

namespace gh_hal::backends::libgpiod_impl {

    VersionString GetLibraryVersion() noexcept {
        return VersionString{gpiod::api_version};
    }

} // namespace gh_hal::backends::libgpiod_impl
