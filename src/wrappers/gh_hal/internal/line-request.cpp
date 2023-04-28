// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_hal/internal/line-request.hpp>

namespace gh_hal::internal {

    LineRequest::LineRequest(consumer_type consumer, chip_reference chip) noexcept :
        m_chipRef{std::move(chip)},
        m_consumer{std::move(consumer)} {}

} // namespace gh_hal::internal
