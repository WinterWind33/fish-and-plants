// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef GH_LOG_LIB_BASE_HPP
#define GH_LOG_LIB_BASE_HPP

#ifdef USE_SPDLOG
#include <spdlog/spdlog.h>
#else
#error "There is no alternative to SPDLOG now."
#endif

#include <string>

namespace gh_log {

    //! \brief Represents the type of the string used by
    //!  loggers.
    using LogStringType = std::string;

} // namespace gh_log

#endif // !GH_LOG_LIB_BASE_HPP
