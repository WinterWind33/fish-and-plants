// Copyright (c) 2023 Andrea Ballestrazzi
#ifdef USE_SPDLOG
#include <gh_log/spl-logger.hpp>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

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

    std::shared_ptr<SPLLogger> SPLLogger::createFileLogger(std::string name, std::filesystem::path filepath) noexcept {
        return std::make_shared<SPLLogger>(spdlog::basic_logger_mt(std::move(name), filepath.string()));
    }

    std::shared_ptr<SPLLogger> SPLLogger::createColoredStdOutLogger(std::string name) noexcept {
        // Color defaulted to automatic.
        return std::make_shared<SPLLogger>(spdlog::stderr_color_mt(std::move(name)));
    }

} // namespace gh_log

#endif // USE_SPDLOG
