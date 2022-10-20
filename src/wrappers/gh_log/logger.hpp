// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef GH_LOG_LOGGER_HPP
#define GH_LOG_LOGGER_HPP

#include <gh_log/gh-log-lib-base.hpp>

namespace gh_log {

//! \brief Represents the basic interface of a logger capable of logging
//!  different levels of severity.
struct Logger {
    virtual ~Logger() noexcept = default;

    virtual void logTrace(const LogStringType& msg) = 0;
    virtual void logDebug(const LogStringType& msg) = 0;
    virtual void logInfo(const LogStringType& msg) = 0;
    virtual void logWarning(const LogStringType& msg) = 0;
    virtual void logError(const LogStringType& msg) = 0;
    virtual void logCritical(const LogStringType& msg) = 0;
};

} // namespace gh_log

#endif // !GH_LOG_LOGGER_HPP
