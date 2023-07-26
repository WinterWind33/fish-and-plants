// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_hal/backends/libgpiod/chip-api.hpp>

// C++ STL
#include <algorithm> // for std::transform
#include <cassert>

namespace gh_hal::backends::libgpiod_impl {

std::unique_ptr<NativeChipType> openChip(const std::filesystem::path& chipPath) {
    return std::make_unique<NativeChipType>(chipPath);
}

namespace details {

[[nodiscard]] static ::gpiod::line_settings createLineSettings(
    const NativeLineDirectionType direction, const bool bUseActiveLowMode) noexcept {
    ::gpiod::line_settings settings{};

    settings.set_direction(direction);
    settings.set_active_low(bUseActiveLowMode);

    return settings;
}

[[nodiscard]] static NativeLineRequestType requestLinesImpl(
    NativeChipType& chip, const std::string& consumer,
    const std::vector<NativeLineOffsetType>& offsets,
    const ::gpiod::line_settings& lineSettings) noexcept {
    assert(static_cast<bool>(chip));

    ::gpiod::line::offsets nativeOffsets{};

    // Now we need to transform the input offsets vector to a gpiod vector
    // because libgpiod stores offsets in a custom class that accepts the value
    // as its only parameter.
    std::transform(offsets.cbegin(), offsets.cend(), std::back_inserter(nativeOffsets),
                   [](const NativeLineOffsetType offsetValue) {
                       return ::gpiod::line::offset{offsetValue};
                   });

    // Now we can perform the native request.
    NativeLineRequestType lineRequest{chip.prepare_request()
                                          .set_consumer(consumer)
                                          .add_line_settings(nativeOffsets, lineSettings)
                                          .do_request()};

    return lineRequest;
}

} // namespace details

NativeLineRequestType requestLines(NativeChipType& chip, const std::string& consumer,
                                   const std::vector<NativeLineOffsetType>& offsets,
                                   const NativeLineDirectionType direction) {
    return details::requestLinesImpl(
        chip, consumer, offsets,
        details::createLineSettings(direction, /*bUseActiveLow = */ false));
}

namespace active_low {
NativeLineRequestType requestLines(NativeChipType& chip, const std::string& consumer,
                                   const std::vector<NativeLineOffsetType>& offsets,
                                   const NativeLineDirectionType direction) {
    return details::requestLinesImpl(
        chip, consumer, offsets, details::createLineSettings(direction, /*bUseActiveLow = */ true));
}
} // namespace active_low

} // namespace gh_hal::backends::libgpiod_impl
