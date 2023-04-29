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

        // Before going on, there shouldn't be any offset inside the requests map.
        for(const auto& lineRequest : m_lineRequests) {
            const auto& offsets{std::get<0>(lineRequest)};

            auto offsetIt = std::find(offsets.cbegin(), offsets.cend(), offset);
            if(offsetIt != offsets.cend()) {
                // This means that an offset has already been requested, we
                // cannot proceed.
                assert(false);
                return nullptr;
            }
        }

        LineRequestKeyComparator::offsets_vector offsets{offset};

        auto newLineRequest{m_lineRequests.emplace(
            offsets,
            LineRequest{std::move(consumer), std::ref(*m_chipPtr), std::move(offsets), direction})};
        // We should have a new insertion, otherwise something went wrong.
        assert(std::get<1>(newLineRequest));

        auto boardPins{std::get<1>(*std::get<0>(newLineRequest)).getBoardPins()};
        // We should have only one board pin.
        assert(boardPins.size() == 1);

        auto boardPin{std::move(boardPins[0])};
        return boardPin;
    }

    std::vector<std::unique_ptr<hardware_access::BoardDigitalPin>> BoardChipImpl::requestDigitalPinPool(std::string consumer,
            std::vector<hardware_access::BoardDigitalPin::offset_type> offsets, const hardware_access::DigitalPinRequestDirection direction) noexcept {
        assert(static_cast<bool>(*m_chipPtr));

        // Before going on, there shouldn't be any offset inside the requests map.
        for(const auto offset : offsets) {
            for(const auto& lineRequest : m_lineRequests) {
                const auto& offsets{std::get<0>(lineRequest)};

                auto offsetIt = std::find(offsets.cbegin(), offsets.cend(), offset);
                if(offsetIt != offsets.cend()) {
                    // This means that an offset has already been requested, we
                    // cannot proceed.
                    assert(false);
                    return {};
                }
            }
        }

        auto newLineRequest{m_lineRequests.emplace(
            offsets,
            LineRequest{std::move(consumer), std::ref(*m_chipPtr), std::move(offsets), direction})};
        // We should have a new insertion, otherwise something went wrong.
        assert(std::get<1>(newLineRequest));

        return std::get<1>(*std::get<0>(newLineRequest)).getBoardPins();
    }

} // namespace gh_hal::internal
