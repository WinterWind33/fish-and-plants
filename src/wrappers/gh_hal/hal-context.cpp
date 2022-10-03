// Copyright (c) 2022 Andrea Ballestrazzi
#include <hal-context.hpp>

namespace gh_hal {

    HALContext::HALContext(logger_pointer logger, const bool bIsSim) noexcept :
        m_logger{std::move(logger)},
        m_bIsSimulation{bIsSim} {}

} // namespace gh_hal
