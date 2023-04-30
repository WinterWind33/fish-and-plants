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

    //!!
    //! \brief Simply creates a new ::gpiod::chip object with the requested chip path.
    //! \note Can throw C++ STL exceptions, like std::system_error etc.
    //!
    //! \param chipPath The path of the chip to open.
    //! \return A unique pointer to a newly opened chip.
    std::unique_ptr<NativeChipType> openChip(std::filesystem::path chipPath);

    //!!
    //! \brief Performs a request for the given lines to the given chip. It converts
    //!  the given offsets to ::gpiod::line::offsets objects first.
    //!
    //! \param chip The chip to which perform the line request.
    //! \param consumer The consumer that will use the requested lines. Can be a simple string with the name
    //!  of the service.
    //! \param offsets The GPIO pins IDs that are going to be requested.
    //! \param direction The direction (input/output) of the requested lines
    //! \return A newly opened line request.
    NativeLineRequestType requestLines(
        NativeChipType& chip,
        std::string consumer,
        std::vector<NativeLineOffsetType> offsets,
        const NativeLineDirectionType direction
    );

} // namespace gh_hal::backends::libgpiod_impl
