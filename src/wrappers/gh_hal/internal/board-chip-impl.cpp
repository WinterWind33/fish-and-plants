// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_hal/internal/board-chip-impl.hpp>

#ifdef USE_LIBGPIOD
#include <gh_hal/backends/libgpiod/chip-api.hpp>
#endif // USE_LIBGPIOD

namespace gh_hal::internal {

    namespace details {
        std::unique_ptr<BackendChipType> openChip(std::filesystem::path chipPath) {
#ifdef USE_LIBGPIOD
            return gh_hal::backends::libgpiod_impl::openChip(std::move(chipPath));
#else
            return nullptr;
#endif // USE_LIBGPIOD
        }
    } // namespace details

    BoardChipImpl::BoardChipImpl(std::filesystem::path chipPath) :
        m_chipPtr{details::openChip(std::move(chipPath))} {}

} // namespace gh_hal::internal
