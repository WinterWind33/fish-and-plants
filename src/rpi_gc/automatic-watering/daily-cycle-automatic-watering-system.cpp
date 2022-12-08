// Copyright (c) 2022 Andrea Ballestrazzi
#include <automatic-watering/daily-cycle-automatic-watering-system.hpp>

#include <common/types.hpp>

// C++ STL
#include <cassert>
#include <format>

namespace rpi_gc::automatic_watering {

    namespace strings {
        namespace feedbacks {
            constexpr StringViewType SHUTDOWN_REQUEST_FEEDBACK{"Shutdown requested for the automatic watering system."};
            constexpr StringViewType EMERGENCY_ABORT_FEEDBACK{"Emergency abort requested."};
            constexpr StringViewType START_WATERING_JOB{"Automatic watering system start requested."};
            constexpr StringViewType AUTOMATIC_WATERING_JOB_START{"Automatic watering system job started."};
            constexpr StringViewType AUTOMATIC_WATERING_JOB_STOP_REQUESTED{"Handling stop request."};
            constexpr StringViewType AUTOMATIC_WATERING_JOB_END{"Automatic watering system job ended."};
        } // namespace feedbacks

        constexpr StringViewType AUTOMATIC_WATERING_SYSTEM_LOG_NAME{"Automatic Watering System"};
    } // namespace strings

    DailyCycleAutomaticWateringSystem::DailyCycleAutomaticWateringSystem(logger_pointer logger) noexcept :
        m_logger{std::move(logger)} {
        assert(m_logger != nullptr);
    }

    void DailyCycleAutomaticWateringSystem::requestShutdown() noexcept {
        m_logger->logInfo(format_log_string(strings::feedbacks::SHUTDOWN_REQUEST_FEEDBACK));

        [[maybe_unused]] const bool bRequestStopSucceded{m_workerThread.request_stop()};
        assert(bRequestStopSucceded);
    }

    void DailyCycleAutomaticWateringSystem::emergencyAbort() noexcept {
        m_logger->logWarning(format_log_string(strings::feedbacks::EMERGENCY_ABORT_FEEDBACK));
    }

    void DailyCycleAutomaticWateringSystem::startAutomaticWatering() noexcept {
        m_logger->logInfo(format_log_string(strings::feedbacks::START_WATERING_JOB));

        m_workerThread = thread_type{[this](std::stop_token stopToken, logger_pointer logger){
            run_automatic_watering(std::move(stopToken), std::move(logger));
        }, m_logger};
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
        return std::format("[{}] {}", strings::AUTOMATIC_WATERING_SYSTEM_LOG_NAME, message);
    }

} // namespace rpi_gc::automatic_watering
