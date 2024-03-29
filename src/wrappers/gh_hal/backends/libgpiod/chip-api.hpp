// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <gpiod.hpp>

// C++ STL
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

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
[[nodiscard]] std::unique_ptr<NativeChipType> openChip(const std::filesystem::path& chipPath);

//!!
//! \brief Performs a request for the given lines to the given chip. It converts
//!  the given offsets to ::gpiod::line::offsets objects first. The final request will be
//!  an "active-low" request, i.e. the I/O pins logics will be inverted.
//!
//! \param chip The chip to which perform the line request.
//! \param consumer The consumer that will use the requested lines. Can be a simple string with the
//! name
//!  of the service.
//! \param offsets The GPIO pins IDs that are going to be requested.
//! \param direction The direction (input/output) of the requested lines
//! \param bIsActiveLow If true, the requested lines will be active-low, i.e. the I/O pins logics
//! will be inverted (On = 0, Off = 1) otherwise it will be active-high.
//! \return A newly opened line request.
[[nodiscard]] NativeLineRequestType requestLines(NativeChipType& chip, const std::string& consumer,
                                                 const std::vector<NativeLineOffsetType>& offsets,
                                                 const NativeLineDirectionType direction,
                                                 const bool bIsActiveLow = true);

} // namespace gh_hal::backends::libgpiod_impl
