// Copyright (c) 2022 Andrea Ballestrazzi
#include <automatic-watering/daily-cycle-automatic-watering-system.hpp>

#include <common/types.hpp>

// C++ STL
#include <cassert>
#include <version>

#ifdef __cpp_lib_format
#include <format>
#else
#include <sstream>
#endif // __cpp_lib_format

namespace rpi_gc::automatic_watering {

    namespace strings {
        namespace feedbacks {
            constexpr StringViewType SHUTDOWN_REQUEST_FEEDBACK{"Shutdown requested for the automatic watering system."};
            constexpr StringViewType EMERGENCY_ABORT_FEEDBACK{"Emergency abort requested."};
            constexpr StringViewType START_WATERING_JOB{"Automatic watering system start requested."};
            constexpr StringViewType AUTOMATIC_WATERING_JOB_START{"Automatic watering system job started."};
            constexpr StringViewType AUTOMATIC_WATERING_JOB_STOP_REQUESTED{"Handling stop request."};
            constexpr StringViewType AUTOMATIC_WATERING_JOB_END{"Automatic watering system job ended."};
            constexpr StringViewType SYSTEM_NOT_RUNNING{"The system is not running."};
        } // namespace feedbacks

        constexpr StringViewType AUTOMATIC_WATERING_SYSTEM_LOG_NAME{"Automatic Watering System"};
    } // namespace strings

    DailyCycleAutomaticWateringSystem::DailyCycleAutomaticWateringSystem(logger_pointer mainLogger, logger_pointer userLogger) noexcept :
        m_mainLogger{std::move(mainLogger)},
        m_userLogger{std::move(userLogger)} {
        assert(m_mainLogger != nullptr);
        assert(m_userLogger != nullptr);
    }

    void DailyCycleAutomaticWateringSystem::requestShutdown() noexcept {
        const StringType formattedLogString{format_log_string(strings::feedbacks::SHUTDOWN_REQUEST_FEEDBACK)};
        m_mainLogger->logInfo(formattedLogString);

        // We also notify the user for this action.
        m_userLogger->logInfo(formattedLogString);
        if(!m_bIsRunning) {
            const StringType systemNotRunning{format_log_string(strings::feedbacks::SYSTEM_NOT_RUNNING)};
            m_mainLogger->logWarning(systemNotRunning);

            // We also notify the user for this action.
            m_userLogger->logWarning(systemNotRunning);
            return;
        }

        [[maybe_unused]] const bool bRequestStopSucceded{m_workerThread.request_stop()};
        assert(bRequestStopSucceded);

        m_workerThread.join();

        m_bIsRunning = false;
    }

    void DailyCycleAutomaticWateringSystem::emergencyAbort() noexcept {
        const StringType formattedLogString{format_log_string(strings::feedbacks::EMERGENCY_ABORT_FEEDBACK)};
        m_mainLogger->logInfo(formattedLogString);

        // We also notify the user for this action.
        m_userLogger->logInfo(formattedLogString);

        if(!m_bIsRunning) {
            const StringType systemNotRunning{format_log_string(strings::feedbacks::SYSTEM_NOT_RUNNING)};
            m_mainLogger->logWarning(systemNotRunning);

            // We also notify the user for this action.
            m_userLogger->logWarning(systemNotRunning);
            return;
        }

        // For now we simply request the thread shutdown.
        m_workerThread.request_stop();
        m_workerThread.join();

        m_bIsRunning = false;
    }

    void DailyCycleAutomaticWateringSystem::startAutomaticWatering() noexcept {
        const StringType formattedLogString{format_log_string(strings::feedbacks::START_WATERING_JOB)};
        m_mainLogger->logInfo(formattedLogString);

        // We also notify the user for this action.
        m_userLogger->logInfo(formattedLogString);

        m_workerThread = thread_type{[this](std::stop_token stopToken, logger_pointer logger){
            run_automatic_watering(std::move(stopToken), std::move(logger));
        }, m_mainLogger};

        m_bIsRunning = true;
    }

    void DailyCycleAutomaticWateringSystem::run_automatic_watering(std::stop_token stopToken, logger_pointer logger) noexcept {
        logger->logInfo(format_log_string(strings::feedbacks::AUTOMATIC_WATERING_JOB_START));
        if(stopToken.stop_requested()) {
            logger->logInfo(format_log_string(strings::feedbacks::AUTOMATIC_WATERING_JOB_STOP_REQUESTED));
        }

        logger->logInfo(format_log_string(strings::feedbacks::AUTOMATIC_WATERING_JOB_END));
    }

    StringType DailyCycleAutomaticWateringSystem::format_log_string(StringViewType message) noexcept {
        // We print the log name for the system as the first argument and then the message.
        // The final string will be:
        // [Automatic Watering System] My custom message.
#ifdef __cpp_lib_format
        return std::format("[{}] {}", strings::AUTOMATIC_WATERING_SYSTEM_LOG_NAME, message);
#else
        std::ostringstream outputStream{};
        outputStream << "[" << StringType{strings::AUTOMATIC_WATERING_SYSTEM_LOG_NAME} << "] ";
        outputStream << StringType{message};

        return outputStream.str();
#endif // __cpp_lib_format
    }

} // namespace rpi_gc::automatic_watering
