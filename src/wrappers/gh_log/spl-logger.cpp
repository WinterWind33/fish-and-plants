// Copyright (c) 2022 Andrea Ballestrazzi
#ifdef USE_SPDLOG
#include <spl-logger.hpp>

// C++ STL
#include <cassert>

namespace gh_log {

SPLLogger::SPLLogger(logger_pointer logger) :
    m_logger{std::move(logger)} {}

void SPLLogger::logTrace(const LogStringType& msg) {
    assert(m_logger);

    m_logger->trace(msg);
}

void SPLLogger::logDebug(const LogStringType& msg) {
    assert(m_logger);

    m_logger->debug(msg);
}

void SPLLogger::logInfo(const LogStringType& msg) {
    assert(m_logger);

    m_logger->info(msg);
}

void SPLLogger::logWarning(const LogStringType& msg) {
    assert(m_logger);

    m_logger->warn(msg);
}

void SPLLogger::logError(const LogStringType& msg) {
    assert(m_logger);

    m_logger->error(msg);
}

void SPLLogger::logCritical(const LogStringType& msg) {
    assert(m_logger);

    m_logger->critical(msg);
}

} // namespace gh_log

#endif // !USE_SPDLOG
