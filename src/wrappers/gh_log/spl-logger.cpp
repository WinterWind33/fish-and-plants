// Copyright (c) 2023 Andrea Ballestrazzi
#ifdef USE_SPDLOG
#include <gh_log/spl-logger.hpp>

//NOLINTNEXTLINE
#include <spdlog/sinks/basic_file_sink.h>
//NOLINTNEXTLINE
#include <spdlog/sinks/stdout_color_sinks.h>
//NOLINTNEXTLINE
#include <spdlog/sinks/daily_file_sink.h>

// C++ STL
#include <cassert>

namespace gh_log {

    namespace details {

        struct LoggingLevelConverter {
            using spdlog_logging_level = spdlog::level::level_enum;
            using map_type = std::array<std::pair<ELoggingLevel, spdlog_logging_level>, spdlog_logging_level::n_levels>;

            static constexpr map_type ToSpdlogMap{
                {
                    {ELoggingLevel::Trace, spdlog_logging_level::trace},
                    {ELoggingLevel::Debug, spdlog_logging_level::debug},
                    {ELoggingLevel::Info, spdlog_logging_level::info},
                    {ELoggingLevel::Warning, spdlog_logging_level::warn},
                    {ELoggingLevel::Error, spdlog_logging_level::err},
                    {ELoggingLevel::Critical, spdlog_logging_level::critical}
                }
            };

            [[nodiscard]]
            static constexpr spdlog_logging_level toSpdlogLevel(const ELoggingLevel level) {
                const auto iter{std::find_if(std::begin(ToSpdlogMap), std::end(ToSpdlogMap), [level](const auto& val){
                    return std::get<0>(val) == level;
                })};

                if (iter != std::end(ToSpdlogMap)) {
                    return std::get<1>(*iter);
                } else {
                    throw std::range_error("SPDLOG Logging level not found!");
                }
            }
        };

        static_assert(LoggingLevelConverter::toSpdlogLevel(ELoggingLevel::Trace) == LoggingLevelConverter::spdlog_logging_level::trace);
        static_assert(LoggingLevelConverter::toSpdlogLevel(ELoggingLevel::Debug) == LoggingLevelConverter::spdlog_logging_level::debug);
        static_assert(LoggingLevelConverter::toSpdlogLevel(ELoggingLevel::Info) == LoggingLevelConverter::spdlog_logging_level::info);
        static_assert(LoggingLevelConverter::toSpdlogLevel(ELoggingLevel::Warning) == LoggingLevelConverter::spdlog_logging_level::warn);
        static_assert(LoggingLevelConverter::toSpdlogLevel(ELoggingLevel::Error) == LoggingLevelConverter::spdlog_logging_level::err);
        static_assert(LoggingLevelConverter::toSpdlogLevel(ELoggingLevel::Critical) == LoggingLevelConverter::spdlog_logging_level::critical);

    } // namespace details

    SPLLogger::SPLLogger(logger_pointer logger) :
        m_logger{std::move(logger)} {
        assert(m_logger);
    }

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

    void SPLLogger::logMessage(const ELoggingLevel level, LogStringType message) {
        m_logger->log(details::LoggingLevelConverter::toSpdlogLevel(level), message);
    }

    void SPLLogger::flush() {
        m_logger->flush();
    }

    void SPLLogger::setAutomaticFlushLevel(const ELoggingLevel level) {
        m_logger->flush_on(details::LoggingLevelConverter::toSpdlogLevel(level));
    }

    std::shared_ptr<SPLLogger> SPLLogger::createFileLogger(const std::string& name, const std::filesystem::path& filepath) noexcept {
        return std::make_shared<SPLLogger>(spdlog::basic_logger_mt(name, filepath.string()));
    }

    std::shared_ptr<SPLLogger> SPLLogger::createColoredStdOutLogger(const std::string& name) noexcept {
        // Color defaulted to automatic.
        return std::make_shared<SPLLogger>(spdlog::stderr_color_mt(name));
    }

    std::shared_ptr<SPLLogger> SPLLogger::createDailyRotatingLogger(const std::string& name) noexcept {
        return std::make_shared<SPLLogger>(spdlog::daily_logger_mt(name, "daily-log.log"));
    }

} // namespace gh_log

#endif // USE_SPDLOG
