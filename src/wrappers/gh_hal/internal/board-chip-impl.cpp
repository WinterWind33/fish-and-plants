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
        libgpiod_impl::NativeLineRequestType requestLines(libgpiod_impl::NativeChipType& chip,
            std::string consumer,
            std::vector<libgpiod_impl::NativeLineOffsetType> offsets,
            const hardware_access::DigitalPinRequestDirection direction) {

            return libgpiod_impl::requestLines(chip, std::move(consumer), std::move(offsets), LibgpiodConverter.convert(direction));
        }

        [[nodiscard]]
        std::unique_ptr<BackendChipType> openChip(std::filesystem::path chipPath) {
            return backends::libgpiod_impl::openChip(std::move(chipPath));
        }

        [[nodiscard]]
        std::vector<std::unique_ptr<hardware_access::BoardDigitalPin>> createDigitalPinsFromLineRequest(::gpiod::line_request& lineRequest) noexcept {
            return {};
        }

#else
        [[nodiscard]]
        std::unique_ptr<BackendChipType> openChip(std::filesystem::path chipPath) {
            return std::make_unique<backends::simulated::SimulatedChip>(std::move(chipPath));
        }

        using FakeLineRequest = std::pair<hardware_access::DigitalPinRequestDirection, std::vector<simulated::DigitalBoardPin>>;

        [[nodiscard]]
        FakeLineRequest requestLines(BackendChipType& chip,
            [[maybe_unused]] std::string /*consumer*/,
            std::vector<hardware_access::BoardDigitalPin::offset_type> offsets,
            [[maybe_unused]] hardware_access::DigitalPinRequestDirection direction) noexcept {

            FakeLineRequest lineRequest{};
            std::get<0>(lineRequest) = direction;

            std::transform(offsets.cbegin(), offsets.cend(), std::back_inserter(std::get<1>(lineRequest)),
                [](const hardware_access::BoardDigitalPin::offset_type offset){
                return simulated::DigitalBoardPin{offset};
            });

            return lineRequest;
        }

        [[nodiscard]]
        std::vector<std::unique_ptr<hardware_access::BoardDigitalPin>> createDigitalPinsFromLineRequest(const FakeLineRequest& lineRequest) noexcept {
            using resulting_vector_type = std::vector<std::unique_ptr<hardware_access::BoardDigitalPin>>;

            resulting_vector_type resultVector{};
            const auto requestDirection{std::get<0>(lineRequest)};
            const auto& simulatedPins{std::get<1>(lineRequest)};

            std::transform(simulatedPins.cbegin(), simulatedPins.cend(), std::back_inserter(resultVector),
                [requestDirection](const simulated::DigitalBoardPin& boardPin){
                return std::make_unique<internal::BoardDigitalPinImpl>(boardPin.getOffsetValue(), requestDirection, boardPin);
            });

            return resultVector;
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

        std::unique_ptr<hardware_access::BoardDigitalPin> boardDigitalPin{};

        auto lineRequest = details::requestLines(*m_chipPtr,
            std::move(consumer),
            {offset},
            direction
        );

        auto boardPins = details::createDigitalPinsFromLineRequest(lineRequest);
        assert(boardPins.size() == 1);

        // We firstly need to move the ownership from vector to a local variable because
        // otherwise we will lose RVO.
        auto boardPin{std::move(boardPins[0])};
        return boardPin;
    }

    std::vector<std::unique_ptr<hardware_access::BoardDigitalPin>> BoardChipImpl::requestDigitalPinPool(std::string consumer,
            std::vector<hardware_access::BoardDigitalPin::offset_type> offset, const hardware_access::DigitalPinRequestDirection direction) noexcept {
        assert(static_cast<bool>(*m_chipPtr));

        auto lineRequest = details::requestLines(*m_chipPtr,
            std::move(consumer),
            {offset},
            direction
        );

        return details::createDigitalPinsFromLineRequest(lineRequest);
    }

} // namespace gh_hal::internal
