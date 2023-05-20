// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// C++ STL
#include <string>

namespace gh_hal::backends::libgpiod_impl {

    //!!
    //! \brief The type of the string that will contain the version
    //!  of the libgpiod library.
    using VersionString = std::string;

    //!!
    //! \brief Retrieves the version of the libgpiod library.
    //!
    //! \return The version of the libgpiod library.
    [[nodiscard]]
    auto GetLibraryVersion() noexcept -> VersionString;

} // namespace gh_hal::backends::libgpiod_impl
