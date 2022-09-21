// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef GH_LOG_LOGGER_HPP
#define GH_LOG_LOGGER_HPP

namespace gh_log {

struct Logger {
    virtual ~Logger() noexcept = default;

    virtual void logTrace() = 0;
    virtual void logDebug() = 0;
    virtual void logInfo() = 0;
    virtual void logWarning() = 0;
    virtual void logError() = 0;
    virtual void logCritical() = 0;
};

} // namespace gh_log

#endif // !GH_LOG_LOGGER_HPP
