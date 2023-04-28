// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <gh_hal/hardware-access/board-digital-pin.hpp>

#ifdef USE_LIBGPIOD
#include <gh_hal/backends/libgpiod/chip-api.hpp>
#endif // USE_LIBGPIOD
#include <gh_hal/backends/simulated/simulated-chip.hpp>
#include <gh_hal/backends/simulated/simulated-digital-board-pin.hpp>

// C++ STL
#include <array>
#include <vector>
#include <memory>
#include <tuple>
#include <type_traits>
#include <cstdint>
#include <algorithm>
#include <functional>

namespace gh_hal::internal {

    namespace details {

        template<typename FromEnum, typename ToEnum, std::size_t EntriesCount>
        class EnumConverter final {
        public:
            constexpr static std::size_t ENTRIES_COUNT{EntriesCount};

            using from_type = std::decay_t<FromEnum>;
            using to_type = std::decay_t<ToEnum>;
            using entry_type = std::pair<from_type, to_type>;
            using map_type = std::array<entry_type, ENTRIES_COUNT>;

            constexpr explicit EnumConverter(map_type entries) noexcept :
                m_conversionMap{std::move(entries)} {}

            [[nodiscard]]
            constexpr to_type convert(const from_type fromValue) const {
                auto enumIt = std::find_if(m_conversionMap.cbegin(), m_conversionMap.cend(),
                    [fromValue](const entry_type& entry){
                        return std::get<0>(entry) == fromValue;
                    });

                if(enumIt != std::cend(m_conversionMap))
                    return std::get<1>(*enumIt);
                else
                    throw std::range_error("Enum entry not recognized.");
            }

        private:
            const map_type m_conversionMap{};
        };

#ifdef USE_LIBGPIOD
        using DirectionEnumConverter = EnumConverter<hardware_access::DigitalPinRequestDirection, ::gpiod::line::direction, 2>;

        constexpr DirectionEnumConverter LibgpiodConverter{
            DirectionEnumConverter::map_type {
                DirectionEnumConverter::entry_type{hardware_access::DigitalPinRequestDirection::Input, ::gpiod::line::direction::INPUT},
                DirectionEnumConverter::entry_type{hardware_access::DigitalPinRequestDirection::Output, ::gpiod::line::direction::OUTPUT}
            }
        };

        static_assert(
            LibgpiodConverter.convert(hardware_access::DigitalPinRequestDirection::Input) == ::gpiod::line::direction::INPUT);
        static_assert(
            LibgpiodConverter.convert(hardware_access::DigitalPinRequestDirection::Output) == ::gpiod::line::direction::OUTPUT);
#endif // USE_LIBGPIOD

        using FakeLineRequest = std::pair<hardware_access::DigitalPinRequestDirection, std::vector<backends::simulated::DigitalBoardPin>>;

    } // namespace details

    class LineRequest final {
    public:
#ifdef USE_LIBGPIOD
        using backend_type = backends::libgpiod_impl::NativeLineRequestType;
        using chip_type = backends::libgpiod_impl::NativeChipType;
#else
        using backend_type = details::FakeLineRequest;
        using chip_type = backends::simulated::SimulatedChip;
#endif // USE_LIBGPIOD

        using consumer_type = std::string;
        using chip_reference = std::reference_wrapper<chip_type>;

        explicit LineRequest(consumer_type consumer, chip_reference chip) noexcept;

    private:
        chip_reference m_chipRef;
        consumer_type m_consumer{};
        std::unique_ptr<backend_type> m_lineRequest{};
    };

} // namespace gh_hal::internal
