// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_hal/internal/line-request.hpp>

#include <gh_hal/internal/board-digital-pin-impl.hpp>

namespace gh_hal::internal {

    LineRequest::LineRequest(
        consumer_type consumer, chip_reference chip, std::vector<offset_type> offsets, const hardware_access::DigitalPinRequestDirection direction) noexcept {
        request_lines(std::move(consumer), std::move(chip), std::move(offsets), direction);
    }

#ifdef USE_LIBGPIOD
    void LineRequest::request_lines(
        consumer_type consumer, chip_reference chip, std::vector<offset_type> offsets, const hardware_access::DigitalPinRequestDirection direction) noexcept {
        try {
            m_lineRequest = std::make_unique<backend_type>(direction,
                backends::libgpiod_impl::requestLines(chip.get(), std::move(consumer), std::move(offsets), details::LibgpiodConverter.convert(direction)));
        } catch(...) {}
    }

    std::vector<std::unique_ptr<hardware_access::BoardDigitalPin>> LineRequest::getBoardPins() const noexcept {
        using resulting_vector_type = std::vector<std::unique_ptr<hardware_access::BoardDigitalPin>>;

        resulting_vector_type result{};

        const ::gpiod::line::offsets offsets{std::get<1>(*m_lineRequest).offsets()};
        const hardware_access::DigitalPinRequestDirection requestedDirection{std::get<0>(*m_lineRequest)};

        std::transform(offsets.cbegin(), offsets.cend(), std::back_inserter(result), [this, requestedDirection](const ::gpiod::line::offset& offset){
            return std::make_unique<internal::BoardDigitalPinImpl>(
                static_cast<internal::BoardDigitalPinImpl::offset_type>(offset), requestedDirection, std::ref(std::get<1>(*m_lineRequest)));
        });

        return result;
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

    std::vector<std::unique_ptr<hardware_access::BoardDigitalPin>> LineRequest::getBoardPins() const noexcept {
        using resulting_vector_type = std::vector<std::unique_ptr<hardware_access::BoardDigitalPin>>;

        resulting_vector_type resultVector{};
        const auto requestDirection{std::get<0>(*m_lineRequest)};
        const auto& simulatedPins{std::get<1>(*m_lineRequest)};

        std::transform(simulatedPins.cbegin(), simulatedPins.cend(), std::back_inserter(resultVector),
            [requestDirection](const backends::simulated::DigitalBoardPin& boardPin){
            return std::make_unique<internal::BoardDigitalPinImpl>(boardPin.getOffsetValue(), requestDirection, boardPin);
        });

        return resultVector;
    }
#endif // USE_LIBGPIOD



} // namespace gh_hal::internal
