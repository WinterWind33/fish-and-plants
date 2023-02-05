// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef LOGGER_MOCK_HPP
#define LOGGER_MOCK_HPP

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

#include <gh_log/logger.hpp>

namespace gh_log::mocks {

    class LoggerMock : public gh_log::Logger {
    public:
        ~LoggerMock() noexcept override = default;

        MOCK_METHOD(void, logTrace, (const LogStringType&), (override));
        MOCK_METHOD(void, logDebug, (const LogStringType&), (override));
        MOCK_METHOD(void, logInfo, (const LogStringType&), (override));
        MOCK_METHOD(void, logWarning, (const LogStringType&), (override));
        MOCK_METHOD(void, logError, (const LogStringType&), (override));
        MOCK_METHOD(void, logCritical, (const LogStringType&), (override));
    };

} // namespace gh_log::mocks

#endif // !LOGGER_MOCK_HPP
