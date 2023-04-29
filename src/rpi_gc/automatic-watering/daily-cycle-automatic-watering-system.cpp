// Copyright (c) 2023 Andrea Ballestrazzi
#include <automatic-watering/daily-cycle-automatic-watering-system.hpp>

#include <common/types.hpp>

// C++ STL
#include <cassert>
#include <version>
#include <chrono>

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

    DailyCycleAutomaticWateringSystem::DailyCycleAutomaticWateringSystem(logger_pointer mainLogger, logger_pointer userLogger,
        hardware_controller_atomic_ref hardwareController, time_provider_atomic_ref timeProvider) noexcept :
        m_mainLogger{std::move(mainLogger)},
        m_userLogger{std::move(userLogger)},
        m_hardwareController{std::move(hardwareController)},
        m_timeProvider{std::move(timeProvider)} {
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

        {
            std::lock_guard<stop_event_mutex> stopLock{m_stopMutex};

            [[maybe_unused]] const bool bRequestStopSucceded{m_workerThread.request_stop()};
            assert(bRequestStopSucceded);
        }

        m_stopListener.notify_one();
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

        {
            std::lock_guard<stop_event_mutex> stopLock{m_stopMutex};

            // For now we simply request the thread shutdown.
            [[maybe_unused]] const bool bRequestStopSucceded{m_workerThread.request_stop()};
            assert(bRequestStopSucceded);
        }
        m_stopListener.notify_one();
        m_workerThread.join();

        m_bIsRunning = false;
    }

    void DailyCycleAutomaticWateringSystem::startAutomaticWatering() noexcept {
        if(m_bIsRunning) {
            const StringType formattedErrorString{
                format_log_string("Automatic watering system already running. Stop the previous instance before starting it again.")};

            m_mainLogger->logError(formattedErrorString);
            m_userLogger->logError(formattedErrorString);

            return;
        }

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
        const time_provider_pointer::value_type timeProvider{m_timeProvider.get().load()};
        assert(timeProvider != nullptr);

        logger->logInfo(format_log_string(strings::feedbacks::AUTOMATIC_WATERING_JOB_START));
        if(stopToken.stop_requested()) {
            logger->logInfo(format_log_string(strings::feedbacks::AUTOMATIC_WATERING_JOB_STOP_REQUESTED));
        }

        std::unique_lock<stop_event_mutex> stopLock{m_stopMutex};
        while(!stopToken.stop_requested()) {
            // It may be possible that the user updated the times, so we need to read them every loop.
            const WateringSystemTimeProvider::time_unit hardwareActivationTime{timeProvider->getWateringSystemActivationDuration()};

            // We start the automatic watering system cycle with the watering on.
            // The watering system lasts for 6 seconds as per requirements.
            activate_watering_hardware();
            m_stopListener.wait_for(stopLock, hardwareActivationTime, [&stopToken](){
                return stopToken.stop_requested();
            });

            if(stopToken.stop_requested()) {
                // If the user requested an abort during th hardware activation
                // we need to deactivate it and exit asap. The watering cycle
                // is interrupted.
                disable_watering_hardware();
                break;
            }

            const WateringSystemTimeProvider::time_unit hardwareDeactivationTime{timeProvider->getWateringSystemDeactivationDuration()};

            // Now we can shut off the hardware.
            disable_watering_hardware();
            m_stopListener.wait_for(stopLock, hardwareDeactivationTime, [&stopToken](){
                return stopToken.stop_requested();
            });
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

    void DailyCycleAutomaticWateringSystem::activate_watering_hardware() noexcept {
        WateringSystemHardwareController::digital_output_type* const waterValveDigitalOut {
            m_hardwareController.get().load()->getWaterValveDigitalOut()
        };
        assert(waterValveDigitalOut != nullptr);

        WateringSystemHardwareController::digital_output_type* const waterPumpDigitalOut {
            m_hardwareController.get().load()->getWaterPumpDigitalOut()
        };
        assert(waterPumpDigitalOut != nullptr);

        // As per requirements for the activation of the watering system we need to activate
        // the water valve before the water pump without waiting.
        m_mainLogger->logInfo(format_log_string("Turning on the water valve."));
        waterValveDigitalOut->activate();

        m_mainLogger->logInfo(format_log_string("Turning on the water pump."));
        waterPumpDigitalOut->activate();
    }

    void DailyCycleAutomaticWateringSystem::disable_watering_hardware() noexcept {
        const time_provider_pointer::value_type timeProvide{m_timeProvider.get().load()};
        assert(timeProvide != nullptr);

        const WateringSystemTimeProvider::time_unit valvePumpSeparationTime{
            timeProvide->getPumpValveDeactivationTimeSeparation()
        };

        WateringSystemHardwareController::digital_output_type* const waterValveDigitalOut {
            m_hardwareController.get().load()->getWaterValveDigitalOut()
        };
        assert(waterValveDigitalOut != nullptr);

        WateringSystemHardwareController::digital_output_type* const waterPumpDigitalOut {
            m_hardwareController.get().load()->getWaterPumpDigitalOut()
        };
        assert(waterPumpDigitalOut != nullptr);

        // As per requirements for the activation of the watering system we need to activate
        // the water valve before the water pump without waiting.
        m_mainLogger->logInfo(format_log_string("Turning off the water valve."));
        waterValveDigitalOut->deactivate();

        std::this_thread::sleep_for(valvePumpSeparationTime);

        m_mainLogger->logInfo(format_log_string("Turning off the water pump."));
        waterPumpDigitalOut->deactivate();
    }

} // namespace rpi_gc::automatic_watering
