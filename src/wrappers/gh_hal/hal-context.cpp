// Copyright (c) 2022 Andrea Ballestrazzi
#include <hal-context.hpp>

namespace gh_hal {

    HALContext::HALContext(logger_pointer logger) noexcept :
        m_logger{std::move(logger)} {}

} // namespace gh_hal
