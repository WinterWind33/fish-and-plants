// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_log/backends/spdlog/spdlog-automatic-flusher.hpp>

// C++ STL
#include <array>
#include <tuple>

namespace gh_log::backends::spdlog_impl {

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

    SpdlogAutomaticFlusher::SpdlogAutomaticFlusher(native_logger_reference nativeLogger) noexcept :
        m_nativeLoggerRef{std::move(nativeLogger)} {}

    void SpdlogAutomaticFlusher::setFlushStartingLevel(const logging_level level) noexcept {
        m_nativeLoggerRef.get().flush_on(details::LoggingLevelConverter::toSpdlogLevel(level));
    }

} // namespace gh_log::backends::spdlog
