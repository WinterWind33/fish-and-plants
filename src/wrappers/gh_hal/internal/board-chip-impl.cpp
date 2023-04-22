// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_hal/internal/board-chip-impl.hpp>

#ifdef USE_LIBGPIOD
#include <gh_hal/backends/libgpiod/chip-api.hpp>
#endif // USE_LIBGPIOD

// C++ STL
#include <cassert>
#include <array>
#include <tuple>
#include <algorithm> // for std::find_if

namespace gh_hal::internal {

    namespace details {
        using namespace gh_hal::backends;

#ifdef USE_LIBGPIOD

        class DirectionEnumConverter final {
        public:
            using gpiod_direction_type = ::gpiod::line::direction;
            using direction_pair = std::pair<hardware_access::DigitalPinRequestDirection, gpiod_direction_type>;

            static constexpr std::array<direction_pair, 2> toBackendMap{
                direction_pair{hardware_access::DigitalPinRequestDirection::Input, gpiod_direction_type::INPUT},
                direction_pair{hardware_access::DigitalPinRequestDirection::Output, gpiod_direction_type::OUTPUT}
            };

            [[nodiscard]] static constexpr gpiod_direction_type convert(const hardware_access::DigitalPinRequestDirection direction) {
                auto dirIt = std::find_if(toBackendMap.cbegin(), toBackendMap.cend(), [direction](const direction_pair& pair){

                    return std::get<0>(pair) == direction;
                });

                if (dirIt != std::cend(toBackendMap))
                    return std::get<1>(*dirIt);
                else
                    throw std::range_error{"Direction not recognized"};
            }
        };

        static_assert(DirectionEnumConverter::convert(hardware_access::DigitalPinRequestDirection::Output) == ::gpiod::line::direction::OUTPUT);
        static_assert(DirectionEnumConverter::convert(hardware_access::DigitalPinRequestDirection::Input) == ::gpiod::line::direction::INPUT);

        libgpiod_impl::NativeLineRequestType requestLines(libgpiod_impl::NativeChipType& chip,
            std::string consumer,
            std::vector<libgpiod_impl::NativeLineOffsetType> offsets,
            const hardware_access::DigitalPinRequestDirection direction) {

            return libgpiod_impl::requestLines(chip, std::move(consumer), std::move(offsets), DirectionEnumConverter::convert(direction));
        }

        std::unique_ptr<BackendChipType> openChip(std::filesystem::path chipPath) {
            return backends::libgpiod_impl::openChip(std::move(chipPath));
        }

#else
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

        std::unique_ptr<hardware_access::BoardDigitalPin> boardDigitalPin{};

        auto lineRequest = details::requestLines(*m_chipPtr,
            std::move(consumer),
            {offset},
            direction
        );

        return boardDigitalPin;
    }

    std::vector<std::unique_ptr<hardware_access::BoardDigitalPin>> BoardChipImpl::requestDigitalPinPool(std::string consumer,
            std::vector<hardware_access::BoardDigitalPin::offset_type> offset, const hardware_access::DigitalPinRequestDirection direction) noexcept {
        return {};
    }

} // namespace gh_hal::internal
