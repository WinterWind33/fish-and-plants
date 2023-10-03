// Copyright (c) 2023 Andrea Ballestrazzi
#include <automatic-watering/daily-cycle-automatic-watering-system.hpp>

#include <common/types.hpp>

// C++ STL
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept> // for std::range_error
#include <string_view>
#include <variant> // for std::bad_variant_access
#include <version>

#ifdef __cpp_lib_format
#include <format>
#else
#include <sstream>
#endif // __cpp_lib_format

namespace rpi_gc::automatic_watering {

namespace strings {
namespace feedbacks {
constexpr StringViewType SHUTDOWN_REQUEST_FEEDBACK{
    "Shutdown requested for the automatic watering system."};
constexpr StringViewType EMERGENCY_ABORT_FEEDBACK{"Emergency abort requested."};
constexpr StringViewType START_WATERING_JOB{"Automatic watering system start requested."};
constexpr StringViewType AUTOMATIC_WATERING_JOB_START{"Automatic watering system job started."};
constexpr StringViewType AUTOMATIC_WATERING_JOB_STOP_REQUESTED{"Handling stop request."};
constexpr StringViewType AUTOMATIC_WATERING_JOB_END{"Automatic watering system job ended."};
constexpr StringViewType SYSTEM_NOT_RUNNING{"The system is not running."};
} // namespace feedbacks

constexpr StringViewType AUTOMATIC_WATERING_SYSTEM_LOG_NAME{"Automatic Watering System"};
} // namespace strings

DailyCycleAutomaticWateringSystem::DailyCycleAutomaticWateringSystem(
    hardware_access_mutex_reference hardwareMutex, logger_pointer mainLogger,
    logger_pointer userLogger, hardware_controller_atomic_ref hardwareController,
    time_provider_atomic_ref timeProvider) noexcept
    : m_mainLogger{std::move(mainLogger)},
      m_userLogger{std::move(userLogger)},
      m_hardwareController{hardwareController},
      m_timeProvider{timeProvider},
      m_hardwareAccessMutex{hardwareMutex} {
    assert(m_mainLogger != nullptr);
    assert(m_userLogger != nullptr);
}

void DailyCycleAutomaticWateringSystem::requestShutdown() noexcept {
    const StringType formattedLogString{
        format_log_string(strings::feedbacks::SHUTDOWN_REQUEST_FEEDBACK)};
    m_mainLogger->logInfo(formattedLogString);

    // We also notify the user for this action.
    m_userLogger->logInfo(formattedLogString);
    if (!isRunning()) {
        const StringType systemNotRunning{
            format_log_string(strings::feedbacks::SYSTEM_NOT_RUNNING)};
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

    m_state.store(EDailyCycleAWSState::Disabled);
    // We reset the cycles counter because it starts when a new
    // cycle is activated.
    m_cyclesCounter.store(0);
}

void DailyCycleAutomaticWateringSystem::emergencyAbort() noexcept {
    const StringType formattedLogString{
        format_log_string(strings::feedbacks::EMERGENCY_ABORT_FEEDBACK)};
    m_mainLogger->logInfo(formattedLogString);

    // We also notify the user for this action.
    m_userLogger->logInfo(formattedLogString);

    if (!isRunning()) {
        const StringType systemNotRunning{
            format_log_string(strings::feedbacks::SYSTEM_NOT_RUNNING)};
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

    m_state.store(EDailyCycleAWSState::Disabled);
    // We reset the cycles counter because it starts when a new
    // cycle is activated.
    m_cyclesCounter.store(0);
}

void DailyCycleAutomaticWateringSystem::startAutomaticWatering() noexcept {
    if (isRunning()) {
        const StringType formattedErrorString{
            format_log_string("Automatic watering system already running. Stop the previous "
                              "instance before starting it again.")};

        m_mainLogger->logError(formattedErrorString);
        m_userLogger->logError(formattedErrorString);

        return;
    }

    // If the user disactivated both the water pump and the water valve then there is no
    // need to proceed and activate the watering system
    if (!m_bWaterValveEnabled.load() && !m_bWaterPumpEnabled.load()) {
        const StringType msgDevicesNotEnabled{
            format_log_string("Both the water valve and the water pump are not enabled.")};
        const StringType msgRunAborted{
            format_log_string("Automatic watering system start aborted.")};
        const StringType msgSuggestion{
            "Consider enabling the water pump and/or the water valve to start a job."};

        m_mainLogger->logWarning(msgDevicesNotEnabled);
        m_userLogger->logWarning(msgDevicesNotEnabled);

        m_mainLogger->logWarning(msgRunAborted);
        m_userLogger->logWarning(msgRunAborted);

        m_userLogger->logInfo(msgSuggestion);

        return;
    }

    const StringType formattedLogString{format_log_string(strings::feedbacks::START_WATERING_JOB)};
    m_mainLogger->logInfo(formattedLogString);

    // We also notify the user for this action.
    m_userLogger->logInfo(formattedLogString);

    m_workerThread = thread_type{[this](std::stop_token stopToken, const logger_pointer& logger) {
                                     run_automatic_watering(std::move(stopToken), logger);
                                 },
                                 m_mainLogger};
}

void DailyCycleAutomaticWateringSystem::run_automatic_watering(
    std::stop_token stopToken, const logger_pointer& logger) noexcept {
    const time_provider_pointer::value_type timeProvider{m_timeProvider.get().load()};
    assert(timeProvider != nullptr);

    // Saving the devices status for later as we need to know if the user
    // decided to deactivate them during a cycle.
    bool bWasValveEnabled{m_bWaterValveEnabled.load()};
    bool bWasPumpEnabled{m_bWaterPumpEnabled.load()};

    logger->logInfo(format_log_string(strings::feedbacks::AUTOMATIC_WATERING_JOB_START));
    if (stopToken.stop_requested()) {
        logger->logInfo(
            format_log_string(strings::feedbacks::AUTOMATIC_WATERING_JOB_STOP_REQUESTED));
    }

    std::unique_lock<stop_event_mutex> stopLock{m_stopMutex};
    while (!stopToken.stop_requested()) {
        // It may be possible that the user updated the times, so we need to read them every loop.
        const WateringSystemTimeProvider::time_unit hardwareActivationTime{
            timeProvider->getWateringSystemActivationDuration()};

        // We start the automatic watering system cycle with the watering on.
        // The watering system lasts for 6 seconds as per requirements.
        activate_watering_hardware();
        m_stopListener.wait_for(stopLock, hardwareActivationTime, [&stopToken]() {
            return stopToken.stop_requested();
        });

        m_state.store(EDailyCycleAWSState::Idling);

        if (stopToken.stop_requested()) {
            // If the user requested an abort during th hardware activation
            // we need to deactivate it and exit asap. The watering cycle
            // is interrupted.
            disable_watering_hardware();
            break;
        }

        // Make sure we deactivate the PINs if the user disabled the devices during the
        // activation.
        auto [bNewValveStatus,
              bNewPumpStatus]{update_devices_status(bWasValveEnabled, bWasPumpEnabled)};
        bWasValveEnabled = bNewValveStatus;
        bWasPumpEnabled = bNewPumpStatus;

        // If both the devices have been disabled, then we don't need to proceed with the job and we
        // can shut it down right now.
        if (!bNewValveStatus && !bNewPumpStatus) {
            constexpr std::string_view FEEDBACK_MESSAGE{
                "Aborting the automatic watering system job as devices have been disabled."};
            m_userLogger->logWarning(format_log_string(FEEDBACK_MESSAGE));
            m_mainLogger->logWarning(format_log_string(FEEDBACK_MESSAGE));

            break;
        }

        const WateringSystemTimeProvider::time_unit hardwareDeactivationTime{
            timeProvider->getWateringSystemDeactivationDuration()};

        // Now we can shut off the hardware.
        disable_watering_hardware();
        m_stopListener.wait_for(stopLock, hardwareDeactivationTime, [&stopToken]() {
            return stopToken.stop_requested();
        });

        m_cyclesCounter++;
    }

    m_state.store(EDailyCycleAWSState::TearingDown);
    logger->logInfo(format_log_string(strings::feedbacks::AUTOMATIC_WATERING_JOB_END));
}

std::pair<bool, bool> DailyCycleAutomaticWateringSystem::update_devices_status(
    const bool bWasValveEnabled, const bool bWasPumpEnabled) noexcept {
    // Here we only need to check whether the device is being disabled. If the device
    // was disabled and then enabled it will wait the next cycle iteration to go up.
    const bool bIsValveEnabled{m_bWaterValveEnabled.load()};
    const bool bHasValveBeenDisabled{bWasValveEnabled && !bIsValveEnabled};

    if (bHasValveBeenDisabled) {
        WateringSystemHardwareController::digital_output_type* const waterValveDigitalOut{
            m_hardwareController.get().load()->getWaterValveDigitalOut()};
        assert(waterValveDigitalOut != nullptr);

        m_mainLogger->logWarning(
            format_log_string("Deactivating the water valve digital out as it has been disabled."));
        waterValveDigitalOut->deactivate();
    }

    const bool bIsPumpEnabled{m_bWaterPumpEnabled.load()};
    const bool bHasPumpBeenDisabled{bWasPumpEnabled && !bIsPumpEnabled};
    if (bHasPumpBeenDisabled) {
        WateringSystemHardwareController::digital_output_type* const waterPumpDigitalOut{
            m_hardwareController.get().load()->getWaterPumpDigitalOut()};
        assert(waterPumpDigitalOut != nullptr);

        m_mainLogger->logWarning(
            format_log_string("Deactivating the water pump digital out as it has been disabled."));
        waterPumpDigitalOut->deactivate();
    }

    return std::make_pair(bIsValveEnabled, bIsPumpEnabled);
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
    std::lock_guard<std::mutex> hardwareLock{m_hardwareAccessMutex};
    m_state.store(EDailyCycleAWSState::Irrigating);

    WateringSystemHardwareController::digital_output_type* const waterValveDigitalOut{
        m_hardwareController.get().load()->getWaterValveDigitalOut()};
    assert(waterValveDigitalOut != nullptr);

    WateringSystemHardwareController::digital_output_type* const waterPumpDigitalOut{
        m_hardwareController.get().load()->getWaterPumpDigitalOut()};
    assert(waterPumpDigitalOut != nullptr);

    // As per requirements for the activation of the watering system we need to activate
    // the water valve before the water pump without waiting.
    std::ostringstream logStream{};

    if (m_bWaterValveEnabled.load()) {
        logStream << format_log_string("Turning on the water valve.") << " ";
        logStream << "[VALVE DIG-OUT] => " << *waterValveDigitalOut;
        m_mainLogger->logInfo(logStream.str());
        waterValveDigitalOut->activate();
    }

    if (m_bWaterPumpEnabled.load()) {
        logStream.str("");
        logStream << format_log_string("Turning on the water pump.") << " ";
        logStream << "[PUMP DIG-OUT] => " << *waterPumpDigitalOut;
        m_mainLogger->logInfo(logStream.str());
        waterPumpDigitalOut->activate();
    }
}

void DailyCycleAutomaticWateringSystem::disable_watering_hardware() noexcept {
    std::lock_guard<std::mutex> hardwareLock{m_hardwareAccessMutex};
    const time_provider_pointer::value_type timeProvide{m_timeProvider.get().load()};
    assert(timeProvide != nullptr);

    const WateringSystemTimeProvider::time_unit valvePumpSeparationTime{
        timeProvide->getPumpValveDeactivationTimeSeparation()};

    WateringSystemHardwareController::digital_output_type* const waterValveDigitalOut{
        m_hardwareController.get().load()->getWaterValveDigitalOut()};
    assert(waterValveDigitalOut != nullptr);

    WateringSystemHardwareController::digital_output_type* const waterPumpDigitalOut{
        m_hardwareController.get().load()->getWaterPumpDigitalOut()};
    assert(waterPumpDigitalOut != nullptr);

    const bool bValveEnabled{m_bWaterValveEnabled.load()};
    const bool bPumpEnabled{m_bWaterPumpEnabled.load()};

    // As per requirements for the activation of the watering system we need to activate
    // the water valve before the water pump without waiting.
    std::ostringstream logStream{};

    if (bValveEnabled) {
        logStream << format_log_string("Turning off the water valve.") << " ";
        logStream << "[VALVE DIG-OUT] => " << *waterValveDigitalOut;
        m_mainLogger->logInfo(logStream.str());
        waterValveDigitalOut->deactivate();
    }

    if (bValveEnabled && bPumpEnabled)
        std::this_thread::sleep_for(valvePumpSeparationTime);

    if (bPumpEnabled) {
        logStream.str("");
        logStream << format_log_string("Turning off the water pump.") << " ";
        logStream << "[PUMP DIG-OUT] => " << *waterPumpDigitalOut;
        m_mainLogger->logInfo(logStream.str());
        waterPumpDigitalOut->deactivate();
    }
}

void DailyCycleAutomaticWateringSystem::setWaterValveEnabled(const bool bEnabled) noexcept {
    m_bWaterValveEnabled.store(bEnabled);
}

void DailyCycleAutomaticWateringSystem::setWaterPumpEnabled(const bool bEnabled) noexcept {
    m_bWaterPumpEnabled.store(bEnabled);
}

namespace details {

struct AWSStateDiagnosticConverter final {
    static constexpr std::array<std::pair<EDailyCycleAWSState, std::string_view>, 4> ConversionMap{
        std::pair{EDailyCycleAWSState::Disabled,    std::string_view{"Disabled"}    },
        std::pair{EDailyCycleAWSState::Idling,      std::string_view{"Idling"}      },
        std::pair{EDailyCycleAWSState::Irrigating,  std::string_view{"Irrigating"}  },
        std::pair{EDailyCycleAWSState::TearingDown, std::string_view{"Tearing Down"}},
    };

    [[nodiscard]] constexpr static std::string_view convertStateToString(
        const EDailyCycleAWSState state) {
        auto msgIt =
            std::find_if(std::cbegin(ConversionMap), std::cend(ConversionMap),
                         [state](const std::pair<EDailyCycleAWSState, std::string_view>& pair) {
                             return std::get<0>(pair) == state;
                         });

        if (msgIt != std::cend(ConversionMap))
            return std::get<1>(*msgIt);
        else
            throw std::range_error("Enum entry not recognized.");
    }
};

std::string ActivationStateToString(
    const WateringSystemHardwareController::activation_state activationState) noexcept {
    using namespace gh_hal::hardware_access;

    switch (activationState) {
        case WateringSystemHardwareController::activation_state::ActiveHigh:
            return "Active High";
        case WateringSystemHardwareController::activation_state::ActiveLow:
            return "Active Low";
        default:
            assert(false);
            return "Unknown";
    }
}

} // namespace details

void DailyCycleAutomaticWateringSystem::printDiagnostic(std::ostream& ost) const noexcept {
    ost << std::endl;
    ost << " [Diagnostic]:\tAutomatic watering system (AWS)" << std::endl;
    ost << " [AWS Mode]:\tCycled" << std::endl;
    try {
        const std::string_view statusStr{
            details::AWSStateDiagnosticConverter::convertStateToString(m_state.load())};

        ost << " [Status]:\t" << statusStr << std::endl;
    } catch (const std::range_error& rangeError) {
        ost << "[ERROR] => " << rangeError.what() << std::endl;
    }

    if (isRunning())
        ost << " [Thread ID]:\t" << m_workerThread.get_id() << std::endl;

    ost << " {AWS Flow}" << std::endl;
    const bool bValveEnabled{m_bWaterValveEnabled.load()};
    const bool bPumpEnabled{m_bWaterPumpEnabled.load()};

    ost << "\t [Completed cycles]: " << m_cyclesCounter.load() << std::endl;

    auto getDeviceStatusStr = [](const bool bEnabled) noexcept -> std::string_view {
        if (bEnabled)
            return "Enabled";

        return "Disabled";
    };

    ost << "\t [Water valve status]: " << getDeviceStatusStr(bValveEnabled) << std::endl;
    ost << "\t [Water pump status]: " << getDeviceStatusStr(bPumpEnabled) << std::endl;

    if (bValveEnabled) {
        ost << "\t [Water valve output PIN]: "
            << m_hardwareController.get().load()->getWaterValveDigitalOut()->getOffset()
            << std::endl
            << "\t [Valve Activation State:] "
            << details::ActivationStateToString(m_hardwareController.get()
                                                    .load()
                                                    ->getWaterValveDigitalOut()
                                                    ->getActivationState())
            << std::endl;
    }

    if (bPumpEnabled) {
        ost << "\t [Water pump output PIN]: "
            << m_hardwareController.get().load()->getWaterPumpDigitalOut()->getOffset() << std::endl
            << "\t [Pump Activation State:] "
            << details::ActivationStateToString(m_hardwareController.get()
                                                    .load()
                                                    ->getWaterPumpDigitalOut()
                                                    ->getActivationState())
            << std::endl;
    }

    ost << "\t [Activation time]:\t"
        << std::chrono::milliseconds{m_timeProvider.get()
                                         .load()
                                         ->getWateringSystemActivationDuration()}
               .count()
        << "ms" << std::endl;
    ost << "\t [Deactivation time]:\t"
        << std::chrono::milliseconds{m_timeProvider.get()
                                         .load()
                                         ->getWateringSystemDeactivationDuration()}
               .count()
        << "ms" << std::endl;
    ost << "\t [Pump-valve sep time]:\t"
        << std::chrono::milliseconds{m_timeProvider.get()
                                         .load()
                                         ->getPumpValveDeactivationTimeSeparation()}
               .count()
        << "ms" << std::endl;
}

void DailyCycleAutomaticWateringSystem::saveToProject(gc::project_management::Project& project) {
    using namespace gc::project_management;
    using namespace std::string_literals;
    ProjectNode awsNode{};
    awsNode.addValue("mode"s, "cycled"s);

    const bool bValveEnabled{m_bWaterValveEnabled.load()};
    const bool bPumpEnabled{m_bWaterPumpEnabled.load()};

    ProjectNode flowNode{};
    std::array<ProjectNode, 2> devicesNodes{};

    ProjectNode valveNode{};

    // We need to put the valve ID and the valve activation
    // state inside the object node.
    valveNode.addValue("name"s, "waterValve"s);

    valveNode.addValue(
        "pinID"s, static_cast<std::uint64_t>(
                      m_hardwareController.get().load()->getWaterValveDigitalOut()->getOffset()));

    valveNode.addValue(
        "activationState"s,
        details::ActivationStateToString(
            m_hardwareController.get().load()->getWaterValveDigitalOut()->getActivationState()));

    valveNode.addValue("enabled"s, bValveEnabled);

    devicesNodes[0] = std::move(valveNode);

    // We need to put the pump ID and the pump activation
    // state inside the object node.
    ProjectNode pumpNode{};

    pumpNode.addValue("name"s, "waterPump"s);

    pumpNode.addValue(
        "pinID"s, static_cast<std::uint64_t>(
                      m_hardwareController.get().load()->getWaterPumpDigitalOut()->getOffset()));

    pumpNode.addValue(
        "activationState"s,
        details::ActivationStateToString(
            m_hardwareController.get().load()->getWaterPumpDigitalOut()->getActivationState()));

    pumpNode.addValue("enabled"s, bPumpEnabled);

    devicesNodes[1] = std::move(pumpNode);

    // Now we need to put the devices nodes inside the flow node.
    flowNode.addObjectArray("devices"s, std::move(devicesNodes));

    flowNode.addValue("activationTime"s,
                      std::chrono::milliseconds{
                          m_timeProvider.get().load()->getWateringSystemActivationDuration()}
                          .count());
    flowNode.addValue("deactivationTime"s,
                      std::chrono::milliseconds{
                          m_timeProvider.get().load()->getWateringSystemDeactivationDuration()}
                          .count());
    flowNode.addValue("deactivationSepTime"s,
                      std::chrono::milliseconds{
                          m_timeProvider.get().load()->getPumpValveDeactivationTimeSeparation()}
                          .count());

    // Now we can put the nodes inside the project.
    awsNode.addObject("flow"s, std::move(flowNode));
    project.addObject("automaticWateringSystem"s, std::move(awsNode));
}

void DailyCycleAutomaticWateringSystem::loadConfigFromProject(
    const gc::project_management::Project& prj) {
    using namespace gc::project_management;
    using namespace std::string_literals;

    // If the given project contains an "automaticWateringSystem" object we can check
    // whether it's cycled mode or not. In the first case we can load the configuration.
    if (!prj.containsObject("automaticWateringSystem"s))
        return;

    const ProjectNode& awsNode{prj.getObject("automaticWateringSystem"s)};

    if (awsNode.getValue<StringType>("mode"s) != "cycled"s) {
        m_userLogger->logError(
            format_log_string("The given project contains an automatic watering system but the "
                              "mode isn\'t recognized. No AWS configuration will be loaded."));
        return;
    }

    if (!awsNode.containsObject("flow"s)) {
        m_userLogger->logWarning(
            format_log_string("The given project doesn\'t contain the flow configuration. Skipping "
                              "configuration loading."));
        return;
    }

    // If the AWS is running we need to stop it before loading the new configuration.
    const bool bWasRunning{isRunning()};
    if (bWasRunning) {
        const StringType formattedLogString{
            format_log_string("Automatic watering system is running. Stopping it before loading "
                              "the new configuration.")};
        m_mainLogger->logWarning(formattedLogString);
        m_userLogger->logWarning(formattedLogString);

        requestShutdown();
    }

    const ProjectNode& flowNode{awsNode.getObject("flow"s)};

    bool bValveEnabled{}, bPumpEnabled{};
    std::uint64_t valvePinID{}, pumpPinID{};
    automatic_watering::WateringSystemHardwareController::activation_state valveActivationState{},
        pumpActivationState{};
    WateringSystemTimeProvider::time_unit activationTime{}, deactivationTime{},
        deactivationSepTime{};

    try {
        // We need to read the devices and associate them to water valve and water pump
        // objects.
        const std::vector<ProjectNode>& devicesNodes{flowNode.getObjectArray("devices"s)};
        for (const ProjectNode& deviceNode : devicesNodes) {
            const StringType deviceName{deviceNode.getValue<StringType>("name"s)};
            if (deviceName == "waterValve"s) {
                bValveEnabled = deviceNode.getValue<bool>("enabled"s);
                valvePinID = deviceNode.getValue<std::uint64_t>("pinID"s);

                // Read the activation state
                const StringType activationStateStr{
                    deviceNode.getValue<StringType>("activationState"s)};
                if (activationStateStr == "Active High"s) {
                    valveActivationState = automatic_watering::WateringSystemHardwareController::
                        activation_state::ActiveHigh;
                } else if (activationStateStr == "Active Low"s) {
                    valveActivationState = automatic_watering::WateringSystemHardwareController::
                        activation_state::ActiveLow;
                }
            } else if (deviceName == "waterPump"s) {
                bPumpEnabled = deviceNode.getValue<bool>("enabled"s);
                pumpPinID = deviceNode.getValue<std::uint64_t>("pinID"s);

                // Read the activation state
                const StringType activationStateStr{
                    deviceNode.getValue<StringType>("activationState"s)};
                if (activationStateStr == "Active High"s) {
                    pumpActivationState = automatic_watering::WateringSystemHardwareController::
                        activation_state::ActiveHigh;
                } else if (activationStateStr == "Active Low"s) {
                    pumpActivationState = automatic_watering::WateringSystemHardwareController::
                        activation_state::ActiveLow;
                }
            } else {
                m_userLogger->logError(
                    format_log_string("The given AWS configuration contains a device name not "
                                      "recognized. No AWS configuration will be loaded."));
                m_userLogger->logWarning(
                    format_log_string("Have you entered wrong values in the configuration?"));

                if (bWasRunning) {
                    const StringType formattedLogString{format_log_string(
                        "Automatic watering system was running. Restoring the old configuration.")};
                    m_mainLogger->logWarning(formattedLogString);
                    m_userLogger->logWarning(formattedLogString);

                    startAutomaticWatering();
                }

                return;
            }
        }

        activationTime =
            std::chrono::milliseconds{flowNode.getValue<std::uint64_t>("activationTime"s)};
        deactivationTime =
            std::chrono::milliseconds{flowNode.getValue<std::uint64_t>("deactivationTime"s)};
        deactivationSepTime =
            std::chrono::milliseconds{flowNode.getValue<std::uint64_t>("deactivationSepTime"s)};
    } catch (const std::bad_variant_access& exc) {
        m_userLogger->logError(
            format_log_string("The given AWS configuration contains a JSON format not recognized. "
                              "No AWS configuration will be loaded."));
        m_userLogger->logWarning(
            format_log_string("Have you entered wrong values in the configuration?"));

        if (bWasRunning) {
            const StringType formattedLogString{format_log_string(
                "Automatic watering system was running. Restoring the old configuration.")};
            m_mainLogger->logWarning(formattedLogString);
            m_userLogger->logWarning(formattedLogString);

            startAutomaticWatering();
        }

        return;
    } catch (const std::exception& exc) {
        m_userLogger->logError(
            format_log_string("Error while loading the AWS configuration. No AWS configuration "
                              "will be loaded. See log for more."));
        m_mainLogger->logError(format_log_string(
            "Error while loading the AWS configuration. No AWS configuration will be loaded."));
        m_mainLogger->logError(format_log_string("Error message: "s + exc.what()));

        if (bWasRunning) {
            const StringType formattedLogString{format_log_string(
                "Automatic watering system was running. Restoring the old configuration.")};
            m_mainLogger->logWarning(formattedLogString);
            m_userLogger->logWarning(formattedLogString);

            startAutomaticWatering();
        }

        return;
    }

    m_bWaterValveEnabled.store(bValveEnabled);
    m_bWaterPumpEnabled.store(bPumpEnabled);

    if (bValveEnabled) {
        m_hardwareController.get().load()->setWaterValveDigitalOutputID(valvePinID,
                                                                        valveActivationState);
    }

    if (bPumpEnabled) {
        m_hardwareController.get().load()->setWaterPumpDigitalOutputID(pumpPinID,
                                                                       pumpActivationState);
    }

    m_timeProvider.get().load()->setWateringSystemActivationDuration(activationTime);
    m_timeProvider.get().load()->setWateringSystemDeactivationDuration(deactivationTime);
    m_timeProvider.get().load()->setPumpValveDeactivationTimeSeparation(deactivationSepTime);

    if (bWasRunning) {
        const StringType formattedLogString{
            format_log_string("Automatic watering system was running. Restarting it.")};
        m_mainLogger->logWarning(formattedLogString);
        m_userLogger->logWarning(formattedLogString);

        startAutomaticWatering();
    }
}

} // namespace rpi_gc::automatic_watering
