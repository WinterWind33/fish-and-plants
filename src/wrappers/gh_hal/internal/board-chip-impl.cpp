// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_hal/internal/board-chip-impl.hpp>
#include <gh_hal/internal/board-digital-pin-impl.hpp>
#include <gh_hal/internal/line-request.hpp>

#ifdef USE_LIBGPIOD
#include <gh_hal/backends/libgpiod/chip-api.hpp>
#else
#include <gh_hal/backends/simulated/simulated-digital-board-pin.hpp>
#endif // USE_LIBGPIOD

// C++ STL
#include <cassert>
#include <vector>
#include <array>
#include <tuple>
#include <algorithm> // for std::find_if

namespace gh_hal::internal {

    namespace details {
        using namespace gh_hal::backends;

#ifdef USE_LIBGPIOD
        [[nodiscard]]
        std::unique_ptr<BackendChipType> openChip(std::filesystem::path chipPath) {
            return backends::libgpiod_impl::openChip(std::move(chipPath));
        }
#else
        [[nodiscard]]
        std::unique_ptr<BackendChipType> openChip(std::filesystem::path chipPath) {
            return std::make_unique<backends::simulated::SimulatedChip>(std::move(chipPath));
        }
#endif // USE_LIBGPIOD
    } // namespace details

    BoardChipImpl::BoardChipImpl(std::filesystem::path chipPath) :
        m_chipPtr{details::openChip(std::move(chipPath))} {}

    BoardChipImpl::operator bool() const noexcept {
        return static_cast<bool>(*m_chipPtr);
    }

    // Line requests

    std::unique_ptr<hardware_access::BoardDigitalPin> BoardChipImpl::requestDigitalPin(std::string consumer,
            hardware_access::BoardDigitalPin::offset_type offset,
            const hardware_access::DigitalPinRequestDirection direction) noexcept {
        assert(static_cast<bool>(*m_chipPtr));

        m_lineRequests.emplace_back(
            std::move(consumer), std::ref(*m_chipPtr), std::vector<hardware_access::BoardDigitalPin::offset_type>{offset}, direction);

        auto boardPins{m_lineRequests.back().getBoardPins()};
        // We should have only one board pin.
        assert(boardPins.size() == 1);

        auto boardPin{std::move(boardPins[0])};
        return boardPin;
    }

    std::vector<std::unique_ptr<hardware_access::BoardDigitalPin>> BoardChipImpl::requestDigitalPinPool(std::string consumer,
            std::vector<hardware_access::BoardDigitalPin::offset_type> offsets, const hardware_access::DigitalPinRequestDirection direction) noexcept {
        assert(static_cast<bool>(*m_chipPtr));

        m_lineRequests.emplace_back(
            std::move(consumer), std::ref(*m_chipPtr), std::move(offsets), direction);

        return m_lineRequests.back().getBoardPins();
    }

} // namespace gh_hal::internal
