// Copyright (C) 2023 Andrea Ballestrazzi
#include <greenhouse-controller-application.hpp>

#include <automatic-watering/daily-cycle-automatic-watering-system.hpp>
#include <automatic-watering/hardware-controllers/daily-cycle-aws-hardware-controller.hpp>
#include <automatic-watering/time-providers/daily-cycle-aws-time-provider.hpp>
#include <automatic-watering/time-providers/configurable-daily-cycle-aws-time-provider.hpp>
#include <gh_log/logger.hpp>
#include <gh_log/spl-logger.hpp>

#include <hardware-management/hardware-chip-initializer.hpp>

// Commands
#include <gh_cmd/gh_cmd.hpp>
#include <commands/application-command.hpp>
#include <commands/abort-command.hpp>
#include <commands/version-command.hpp>
#include <commands/help-command.hpp>
#include <commands/automatic-watering/automatic-watering-command.hpp>

#include <gh_hal/hal-context.hpp>
#include <gh_hal/hal-error.hpp>

// C++ STL
#include <iostream>
#include <memory>
#include <algorithm>
#include <atomic>
#include <mutex>
#include <limits>

namespace automatic_watering {

    std::shared_ptr<rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider>
        CreateConfigurableAWSTimeProvider() noexcept {
        rpi_gc::automatic_watering::DailyCycleAWSTimeProvider defaultTimeProvider{};

        return std::make_shared<rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider>(
            defaultTimeProvider.getWateringSystemActivationDuration(),
            defaultTimeProvider.getWateringSystemDeactivationDuration(),
            defaultTimeProvider.getPumpValveDeactivationTimeSeparation()
        );
    }

} // namespace automatic_watering


namespace constants {

    constexpr rpi_gc::automatic_watering::DailyCycleAWSHardwareController::digital_output_id WATER_VALVE_PIN_ID{26};
    constexpr rpi_gc::automatic_watering::DailyCycleAWSHardwareController::digital_output_id WATER_PUMP_PIN_ID{23};

} // namespace constants

// test changesasdasd

namespace commands_factory {

    template<typename WateringSystemPointer, typename OptionParserType>
    [[nodiscard]] static std::unique_ptr<rpi_gc::AutomaticWateringCommand>
        CreateAutomaticWateringCommand(WateringSystemPointer wateringSystem) {
        using rpi_gc::AutomaticWateringCommand;
        using rpi_gc::CharType;

        assert(static_cast<bool>(wateringSystem));

        rpi_gc::automatic_watering::DailyCycleAWSTimeProvider defaultTimeProvider{};

        AutomaticWateringCommand::option_parser_pointer autoWateringOptionParser{std::make_unique<OptionParserType>("[OPTIONS] => auto-watering")};
        autoWateringOptionParser->addSwitch(std::make_shared<gh_cmd::Switch<CharType>>('h', "help", "Displays this help page."));
        autoWateringOptionParser->addSwitch(std::make_shared<gh_cmd::Switch<CharType>>('S', "start", "Starts the automatic watering system in Daily-Cycle mode."));
        autoWateringOptionParser->addSwitch(std::make_shared<gh_cmd::Switch<CharType>>('s', "stop", "Stops the automatic watering system waiting for resources to be released."));
        autoWateringOptionParser->addOption(
            std::make_shared<gh_cmd::Value<CharType, rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider::rep_type>>(
                'A', "activation-time", "Sets the automatic watering system activation time (expressed in ms).", defaultTimeProvider.getWateringSystemActivationDuration().count()
            ));
        autoWateringOptionParser->addOption(
            std::make_shared<gh_cmd::Value<CharType, rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider::rep_type>>(
                'D', "deactivation-time", "Sets the automatic watering system deactivation time (expressed in ms).", defaultTimeProvider.getWateringSystemDeactivationDuration().count()
            ));

        autoWateringOptionParser->addOption(
            std::make_shared<gh_cmd::Value<CharType, rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider::rep_type>>(
                'P', "pumpvalve-deactsep-time", "(CAREFUL) Sets the separation time (espressed in ms) between the pump deactivation and the valve one. " \
                "WARNING: PLAYING WITH THIS CONFIGURATION MAY LEAD TO HARDWARE FAILURE. USE WITH CAUTION.",
                defaultTimeProvider.getPumpValveDeactivationTimeSeparation().count()
            ));

        autoWateringOptionParser->addOption(
            std::make_shared<gh_cmd::Value<CharType, rpi_gc::automatic_watering::DailyCycleAWSHardwareController::digital_output_id>>(
                'V', "valve-pin-id", "Sets the ID of the pin which will receive the output jumpers that will power up the water valve.",
                constants::WATER_VALVE_PIN_ID
            ));

        autoWateringOptionParser->addOption(
            std::make_shared<gh_cmd::Value<CharType, rpi_gc::automatic_watering::DailyCycleAWSHardwareController::digital_output_id>>(
                'U', "pump-pin-id", "Sets the ID of the pin which will receive the output jumpers that will power up the water pump.",
                constants::WATER_PUMP_PIN_ID
            ));

        std::unique_ptr<AutomaticWateringCommand> autoWateringCommand{std::make_unique<AutomaticWateringCommand>(std::cout, std::move(autoWateringOptionParser))};
        autoWateringCommand->registerOptionEvent(
            "start",
            [wateringSystem]([[maybe_unused]] const AutomaticWateringCommand::option_parser::const_option_pointer&) {
                wateringSystem->startAutomaticWatering();
            }
        );
        autoWateringCommand->registerOptionEvent(
            "stop",
            [wateringSystem]([[maybe_unused]] const AutomaticWateringCommand::option_parser::const_option_pointer&) {
                wateringSystem->requestShutdown();
            }
        );

        return autoWateringCommand;
    }

} // namespace commands_factory

namespace hardware_chip_paths {

    constexpr std::string_view RASPBERRY_PI_3B_PLUS_CHIP_PATH_GPIO0{"/dev/gpiochip0"};

} // namespace hardware_chip_paths

namespace details {

    template<typename T>
    T ClampWithError(const T value, const T minVal, const T maxVal, gh_log::Logger& logger, const std::string& errorMessage) noexcept {
        T clampedValue{std::clamp(value, minVal, maxVal)};

        if(clampedValue != value)
            logger.logError(errorMessage);

        return clampedValue;
    }

} // namespace details

// This is the entry point of the application. Here, it starts
// the main execution of the greenhouse controller.
int main(int argc, char* argv[]) {
    using namespace rpi_gc;

    using DefaultOptionParser = gh_cmd::DefaultOptionParser<CharType>;
    using ApplicationOptionParser = DefaultOptionParser;
    using LoggerPointer = std::shared_ptr<gh_log::Logger>;

    LoggerPointer mainLogger{gh_log::SPLLogger::createFileLogger(
        StringType{strings::application::NAME},
        strings::application::MAIN_LOG_FILENAME
    )};
    mainLogger->setAutomaticFlushLevel(gh_log::ELoggingLevel::Info);
    mainLogger->logInfo("Initiating system: starting log now.");

    LoggerPointer userLogger{gh_log::SPLLogger::createColoredStdOutLogger("Reporter")};
    userLogger->setAutomaticFlushLevel(gh_log::ELoggingLevel::Info);

    OutputStringStream applicationHelpStream{};

    mainLogger->logInfo("Initiating hardware abstraction layer.");
    rpi_gc::hardware_management::HardwareInitializer<gh_hal::hardware_access::BoardChipFactory> hardwareInitializer{mainLogger};

    std::unique_ptr<gh_hal::hardware_access::BoardChip> boardChip{};
    try {
        // We try to initialize the board chip. If this doesn't go well we can't
        // proceed with the application process.
        boardChip = hardwareInitializer.initializeBoardChip(std::filesystem::path{hardware_chip_paths::RASPBERRY_PI_3B_PLUS_CHIP_PATH_GPIO0});
    } catch(const std::exception& hardwareInitializationError) {
        constexpr std::string_view ABORTING_MESSAGE{"Aborting the process. Return code: -1."};

        std::ostringstream userFeedbackStream{};
        userFeedbackStream << "Failed to initialize the hardware abstraction layer. ";
        userFeedbackStream << "Message: " << hardwareInitializationError.what() << ' ';

        userLogger->logError(userFeedbackStream.str());
        userLogger->logWarning("See the log file for more details.");
        userLogger->logInfo(std::string{ABORTING_MESSAGE});
        mainLogger->logInfo(std::string{ABORTING_MESSAGE});

        return -1;
    }

    auto awsTimeProviderSmartPtr{::automatic_watering::CreateConfigurableAWSTimeProvider()};
    std::mutex awsHardwareAccessMutex{};
    rpi_gc::automatic_watering::DailyCycleAutomaticWateringSystem::time_provider_pointer awsTimeProvider{
        awsTimeProviderSmartPtr.get()
    };

    using AutomaticWateringSystemPointer = std::shared_ptr<rpi_gc::automatic_watering::DailyCycleAutomaticWateringSystem>;
    std::unique_ptr<rpi_gc::automatic_watering::DailyCycleAWSHardwareController> awsHardwareController{};

    mainLogger->logInfo("Initiating the automatic watering hardware controller.");
    awsHardwareController = std::make_unique<rpi_gc::automatic_watering::DailyCycleAWSHardwareController>(
        std::ref(awsHardwareAccessMutex), std::ref(*boardChip), constants::WATER_VALVE_PIN_ID, constants::WATER_PUMP_PIN_ID);

    std::atomic<rpi_gc::automatic_watering::WateringSystemHardwareController*> hardwareControllerAtomic{awsHardwareController.get()};

    mainLogger->logInfo("Initiating the automatic watering system.");
    AutomaticWateringSystemPointer automaticWateringSystem{
        std::make_shared<rpi_gc::automatic_watering::DailyCycleAutomaticWateringSystem>(
            std::ref(awsHardwareAccessMutex),
            mainLogger,
            userLogger,
            std::ref(hardwareControllerAtomic),
            std::ref(awsTimeProvider)
        )
    };

    mainLogger->logInfo("Initiating application commands and user interface...");
    auto versionCommand = std::make_unique<VersionCommand>(std::cout);

    auto autoWateringCommand = commands_factory::CreateAutomaticWateringCommand<AutomaticWateringSystemPointer, DefaultOptionParser>(
            automaticWateringSystem
    );

    autoWateringCommand->registerOptionEvent(
        "activation-time",
        [&awsTimeProvider, mainLogger, userLogger](const AutomaticWateringCommand::option_parser::const_option_pointer& option) {
            auto valueOption = std::static_pointer_cast<
                const gh_cmd::Value<CharType, rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider::rep_type>>(option);

            assert(static_cast<bool>(valueOption));

            const auto value{valueOption->value()};
            const auto clampedValue{
                details::ClampWithError<decltype(value)>(
                    value, 0, std::numeric_limits<std::int64_t>::max(), *userLogger, "The activation time is out of the acceptable range [0, inf]. The value will be clamped.")};

            std::ostringstream formatString{};
            formatString << "Automatic watering system activation time set to ";
            formatString << clampedValue;
            formatString << "ms.";

            auto* const timeProvider = dynamic_cast<rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider*>(awsTimeProvider.load());
            timeProvider->setActivationTimeTicks(clampedValue);

            userLogger->logInfo(formatString.str());
            mainLogger->logInfo(formatString.str());
        }
    );

    autoWateringCommand->registerOptionEvent(
        "deactivation-time",
        [&awsTimeProvider, mainLogger, userLogger](const AutomaticWateringCommand::option_parser::const_option_pointer& option) {
            auto valueOption = std::static_pointer_cast<
                const gh_cmd::Value<CharType, rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider::rep_type>>(option);

            assert(static_cast<bool>(valueOption));

            const auto value{valueOption->value()};
            const auto clampedValue{
                details::ClampWithError<decltype(value)>(
                    value, 0, std::numeric_limits<std::int64_t>::max(), *userLogger, "The deactivation time is out of the acceptable range [0, inf]. The value will be clamped.")};

            std::ostringstream formatString{};
            formatString << "Automatic watering system deactivation time set to ";
            formatString << clampedValue;
            formatString << "ms.";

            auto* const timeProvider = dynamic_cast<rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider*>(awsTimeProvider.load());
            timeProvider->setDeactivationTimeTicks(clampedValue);

            userLogger->logInfo(formatString.str());
            mainLogger->logInfo(formatString.str());
        }
    );

    autoWateringCommand->registerOptionEvent(
        "pumpvalve-deactsep-time",
        [&awsTimeProvider, mainLogger, userLogger](const AutomaticWateringCommand::option_parser::const_option_pointer& option) {
            auto valueOption = std::static_pointer_cast<
                const gh_cmd::Value<CharType, rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider::rep_type>>(option);

            assert(static_cast<bool>(valueOption));

            const auto value{valueOption->value()};
            const auto clampedValue{
                details::ClampWithError<decltype(value)>(
                    value, 0, std::numeric_limits<std::int64_t>::max(), *userLogger, "The separation time is out of the acceptable range [0, inf]. The value will be clamped.")};

            std::ostringstream formatString{};
            formatString << "Pump-valve deactivation separation time set to ";
            formatString << clampedValue;
            formatString << "ms.";

            auto* const timeProvider = dynamic_cast<rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider*>(awsTimeProvider.load());
            timeProvider->setPumpValveWaitTimeTicks(clampedValue);

            userLogger->logWarning(formatString.str());
            mainLogger->logWarning(formatString.str());
        }
    );

    autoWateringCommand->registerOptionEvent(
        "valve-pin-id",
        [&hardwareControllerAtomic, mainLogger, userLogger](const AutomaticWateringCommand::option_parser::const_option_pointer& option) {
            auto valueOption = std::static_pointer_cast<
                const gh_cmd::Value<CharType, rpi_gc::automatic_watering::DailyCycleAWSHardwareController::digital_output_id>>(option);

            assert(static_cast<bool>(valueOption));
            auto* const hardwareController = dynamic_cast<rpi_gc::automatic_watering::DailyCycleAWSHardwareController*>(hardwareControllerAtomic.load());

            hardwareController->setWaterValveDigitalOutputID(valueOption->value());

            std::ostringstream formatString{};
            formatString << "Received new valve pin ID: ";
            formatString << valueOption->value();

            userLogger->logWarning(formatString.str());
            mainLogger->logWarning(formatString.str());
        }
    );

    autoWateringCommand->registerOptionEvent(
        "pump-pin-id",
        [&hardwareControllerAtomic, mainLogger, userLogger](const AutomaticWateringCommand::option_parser::const_option_pointer& option) {
            auto valueOption = std::static_pointer_cast<
                const gh_cmd::Value<CharType, rpi_gc::automatic_watering::DailyCycleAWSHardwareController::digital_output_id>>(option);

            assert(static_cast<bool>(valueOption));
            auto* const hardwareController = dynamic_cast<rpi_gc::automatic_watering::DailyCycleAWSHardwareController*>(hardwareControllerAtomic.load());

            hardwareController->setWaterPumpDigitalOutputID(valueOption->value());

            std::ostringstream formatString{};
            formatString << "Received new pump pin ID: ";
            formatString << valueOption->value();

            userLogger->logWarning(formatString.str());
            mainLogger->logWarning(formatString.str());
        }
    );

    auto abortCommandOptionParser{std::make_unique<gh_cmd::DefaultOptionParser<CharType>>("[OPTIONS] => abort")};
    abortCommandOptionParser->addSwitch(std::make_shared<gh_cmd::Switch<CharType>>('h', "help", "Displays this help page."));
    auto abortCommand = std::make_unique<commands::AbortCommand>(mainLogger,
        std::vector<commands::AbortCommand::emergency_stoppable_system_pointer>{automaticWateringSystem},
        std::move(abortCommandOptionParser)
    );

    auto helpCommand = std::make_unique<HelpCommand>(
        std::cout,
        std::vector<HelpCommand::terminal_command_const_ref>{
            std::cref(*versionCommand),
            std::cref(*autoWateringCommand),
            std::cref(*abortCommand)
        }
    );

    auto applicationOptionParser = std::make_unique<ApplicationOptionParser>("rpi_gc command options");
    auto applicationCommand = std::make_unique<ApplicationCommand>(std::cout, *applicationOptionParser);
    applicationCommand->addBivalentCommand(*versionCommand);
    applicationCommand->addBivalentCommand(*helpCommand);

    applicationOptionParser->printHelp(applicationHelpStream);
    helpCommand->setApplicationHelp(applicationHelpStream.str());

    mainLogger->logInfo("Initiating main application controller...");
    GreenhouseControllerApplication mainApplication{std::cout, std::cin, mainLogger};
    mainApplication.addSupportedCommand(std::move(autoWateringCommand));
    mainApplication.addSupportedCommand(std::move(abortCommand));
    mainApplication.addSupportedCommand(std::move(versionCommand));
    mainApplication.addSupportedCommand(std::move(helpCommand));
    mainApplication.setApplicationCommand(std::move(applicationCommand));

    mainApplication.addTerminableSystem(std::move(automaticWateringSystem));

    if(!mainApplication.processInputOptions(argc, argv))
        return 1;

    mainLogger->logInfo("Starting application loop.");
    mainApplication.run();

    mainLogger->logInfo("Exiting now [Result: 0].");
    return 0;
}
