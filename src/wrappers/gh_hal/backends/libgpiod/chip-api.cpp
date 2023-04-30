// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_hal/backends/libgpiod/chip-api.hpp>

// C++ STL
#include <algorithm> // for std::transform
#include <cassert>

namespace gh_hal::backends::libgpiod_impl {

    std::unique_ptr<NativeChipType> openChip(std::filesystem::path chipPath) {
        return std::make_unique<NativeChipType>(chipPath);
    }

    NativeLineRequestType requestLines(NativeChipType& chip, std::string consumer, std::vector<NativeLineOffsetType> offsets,
        const NativeLineDirectionType direction) {
        assert(static_cast<bool>(chip));

        ::gpiod::line::offsets nativeOffsets{};

        // Now we need to transform the input offsets vector to a gpiod vector
        // because libgpiod stores offsets in a custom class that accepts the value
        // as its only parameter.
        std::transform(offsets.cbegin(), offsets.cend(), std::back_inserter(nativeOffsets), [](const NativeLineOffsetType offsetValue){
            return ::gpiod::line::offset{offsetValue};
        });

        // Now we can perform the native request.
        NativeLineRequestType lineRequest{
            chip.prepare_request()
                .set_consumer(consumer)
                .add_line_settings(
                    nativeOffsets,
                    ::gpiod::line_settings{}
                        .set_direction(direction)
                )
                .do_request()
        };

        return lineRequest;
    }

} // namespace gh_hal::backends::libgpiod_impl
