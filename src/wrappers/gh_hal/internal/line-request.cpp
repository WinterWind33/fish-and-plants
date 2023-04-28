// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_hal/internal/line-request.hpp>

namespace gh_hal::internal {

    LineRequest::LineRequest(
        consumer_type consumer, chip_reference chip, std::vector<offset_type> offsets, const hardware_access::DigitalPinRequestDirection direction) noexcept {
        request_lines(std::move(consumer), std::move(chip), std::move(offsets), direction);
    }

#ifdef USE_LIBGPIOD
    void LineRequest::request_lines(
        consumer_type consumer, chip_reference chip, std::vector<offset_type> offsets, const hardware_access::DigitalPinRequestDirection direction) noexcept {
        try {
            m_lineRequest = std::make_unique<backend_type>(
                backends::libgpiod_impl::requestLines(chip.get(), std::move(consumer), std::move(offsets), details::LibgpiodConverter.convert(direction)));
        } catch(...) {}
    }

#else

    void LineRequest::request_lines(
        consumer_type consumer, chip_reference chip, std::vector<offset_type> offsets, const hardware_access::DigitalPinRequestDirection direction) noexcept {
        m_lineRequest = std::make_unique<backend_type>(direction, std::vector<backends::simulated::DigitalBoardPin>{});

        std::transform(offsets.cbegin(), offsets.cend(), std::back_inserter(std::get<1>(*m_lineRequest)),
            [](const hardware_access::BoardDigitalPin::offset_type offset){
            return backends::simulated::DigitalBoardPin{offset};
        });
    }
#endif // USE_LIBGPIOD
} // namespace gh_hal::internal
