// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

//#ifdef USE_SPDLOG

#include <gh_log/implementation/automatic-flusher.hpp>
#include <gh_log/logger.hpp>

#include <spdlog/spdlog.h>

// C++ STL
#include <functional> // for std::reference_wrapper

namespace gh_log::backends::spdlog_impl {

    class SpdlogAutomaticFlusher final : public gh_log::implementation::AutomaticFlusher<gh_log::ELoggingLevel> {
    public:
        using native_logger_reference = std::reference_wrapper<spdlog::logger>;

        explicit SpdlogAutomaticFlusher(native_logger_reference nativeLogger) noexcept;

        void setFlushStartingLevel(const logging_level level) noexcept override;
    private:
        native_logger_reference m_nativeLoggerRef;
    };

} // namespace gh_log::backends::spdlog_impl

//#endif // USE_SPDLOG
