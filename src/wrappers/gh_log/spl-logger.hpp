// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef SPL_LOGGER_HPP
#define SPL_LOGGER_HPP

#ifdef USE_SPDLOG
#include <spdlog/spdlog.h>
#include <gh_log/logger.hpp>

// C++ STL
#include <memory>
#include <filesystem>

namespace gh_log {

    //! \brief Represents a logger that delegates the implementation to
    //!  an spdlog logger object.
    class SPLLogger : public Logger {
    public:
        using logger_type = spdlog::logger;
        using logger_pointer = std::shared_ptr<logger_type>;
        using sink_type = spdlog::sinks::sink;
        using sink_pointer = std::shared_ptr<sink_type>;

        SPLLogger(logger_pointer logger);
        ~SPLLogger() noexcept override = default;

        [[nodiscard]]
        static std::shared_ptr<SPLLogger> createFileLogger(std::string name, std::filesystem::path filepath) noexcept;

        [[nodiscard]]
        static std::shared_ptr<SPLLogger> createColoredStdOutLogger(std::string name) noexcept;

        void logTrace(const LogStringType& msg) override;
        void logDebug(const LogStringType& msg) override;
        void logInfo(const LogStringType& msg) override;
        void logWarning(const LogStringType& msg) override;
        void logError(const LogStringType& msg) override;
        void logCritical(const LogStringType& msg) override;

    private:
        logger_pointer m_logger{};
    };

} // namespace gh_log

#endif // USE_SPDLOG
#endif // !SPL_LOGGER_HPP
