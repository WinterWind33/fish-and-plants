// Copyright (C) 2023 Andrea Ballestrazzi
#include <greenhouse-controller-application.hpp>

#include <automatic-watering/daily-cycle-automatic-watering-system.hpp>
#include <automatic-watering/hardware-controllers/daily-cycle-aws-hardware-controller.hpp>
#include <automatic-watering/time-providers/daily-cycle-aws-time-provider.hpp>
#include <automatic-watering/time-providers/configurable-daily-cycle-aws-time-provider.hpp>
#include <gh_log/logger.hpp>
#include <gh_log/spl-logger.hpp>

// Commands
#include <gh_cmd/gh_cmd.hpp>
#include <commands/application-command.hpp>
#include <commands/abort-command.hpp>
#include <commands/version-command.hpp>
#include <commands/help-command.hpp>
#include <commands/automatic-watering/automatic-watering-command.hpp>

#include <gh_hal/hal-context.hpp>

// C++ STL
#include <iostream>
#include <memory>
#include <algorithm>
#include <atomic>

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

        assert((bool)wateringSystem);

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
            [wateringSystem]([[maybe_unused]] AutomaticWateringCommand::option_parser::const_option_pointer) {
                wateringSystem->startAutomaticWatering();
            }
        );
        autoWateringCommand->registerOptionEvent(
            "stop",
            [wateringSystem]([[maybe_unused]] AutomaticWateringCommand::option_parser::const_option_pointer) {
                wateringSystem->requestShutdown();
            }
        );

        return autoWateringCommand;
    }

} // namespace commands_factory

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

    mainLogger->logInfo("Initiating system: starting log now.");

    LoggerPointer userLogger{gh_log::SPLLogger::createColoredStdOutLogger("Reporter")};
    OutputStringStream applicationHelpStream{};

    mainLogger->logWarning("Initiating hardware abstraction layer.");
    gh_hal::HALContext halContext{mainLogger, false};

    auto awsTimeProviderSmartPtr{::automatic_watering::CreateConfigurableAWSTimeProvider()};

    rpi_gc::automatic_watering::DailyCycleAutomaticWateringSystem::time_provider_pointer awsTimeProvider{
        awsTimeProviderSmartPtr.get()
    };

    using AutomaticWateringSystemPointer = std::shared_ptr<rpi_gc::automatic_watering::DailyCycleAutomaticWateringSystem>;
    auto awsHardwareController = std::make_unique<rpi_gc::automatic_watering::DailyCycleAWSHardwareController>(constants::WATER_VALVE_PIN_ID, constants::WATER_PUMP_PIN_ID);
    std::atomic<rpi_gc::automatic_watering::WateringSystemHardwareController*> hardwareControllerAtomic{awsHardwareController.get()};

    AutomaticWateringSystemPointer automaticWateringSystem{
        std::make_shared<rpi_gc::automatic_watering::DailyCycleAutomaticWateringSystem>(
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
        [&awsTimeProvider, mainLogger, userLogger](AutomaticWateringCommand::option_parser::const_option_pointer option) {
            auto valueOption = std::static_pointer_cast<
                const gh_cmd::Value<CharType, rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider::rep_type>>(option);

            assert(static_cast<bool>(valueOption));
            auto* const timeProvider = static_cast<rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider*>(awsTimeProvider.load());
            timeProvider->setActivationTimeTicks(valueOption->value());

            std::ostringstream formatString{};
            formatString << "Received new automatic watering system activation time: ";
            formatString << valueOption->value();
            formatString << "ms.";

            userLogger->logInfo(formatString.str());
            mainLogger->logInfo(formatString.str());
        }
    );

    autoWateringCommand->registerOptionEvent(
        "deactivation-time",
        [&awsTimeProvider, mainLogger, userLogger](AutomaticWateringCommand::option_parser::const_option_pointer option) {
            auto valueOption = std::static_pointer_cast<
                const gh_cmd::Value<CharType, rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider::rep_type>>(option);

            assert(static_cast<bool>(valueOption));
            auto* const timeProvider = static_cast<rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider*>(awsTimeProvider.load());
            timeProvider->setDeactivationTimeTicks(valueOption->value());

            std::ostringstream formatString{};
            formatString << "Received new automatic watering system deactivation time: ";
            formatString << valueOption->value();
            formatString << "ms.";

            userLogger->logInfo(formatString.str());
            mainLogger->logInfo(formatString.str());
        }
    );

    autoWateringCommand->registerOptionEvent(
        "pumpvalve-deactsep-time",
        [&awsTimeProvider, mainLogger, userLogger](AutomaticWateringCommand::option_parser::const_option_pointer option) {
            auto valueOption = std::static_pointer_cast<
                const gh_cmd::Value<CharType, rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider::rep_type>>(option);

            assert(static_cast<bool>(valueOption));
            auto* const timeProvider = static_cast<rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider*>(awsTimeProvider.load());
            timeProvider->setPumpValveWaitTimeTicks(valueOption->value());

            std::ostringstream formatString{};
            formatString << "Received new pump-valve deactivation separation time: ";
            formatString << valueOption->value();
            formatString << "ms.";

            userLogger->logWarning(formatString.str());
            mainLogger->logWarning(formatString.str());
        }
    );

    autoWateringCommand->registerOptionEvent(
        "valve-pin-id",
        [&hardwareControllerAtomic, mainLogger, userLogger](AutomaticWateringCommand::option_parser::const_option_pointer option) {
            auto valueOption = std::static_pointer_cast<
                const gh_cmd::Value<CharType, rpi_gc::automatic_watering::DailyCycleAWSHardwareController::digital_output_id>>(option);

            assert(static_cast<bool>(valueOption));
            auto* const hardwareController = static_cast<rpi_gc::automatic_watering::DailyCycleAWSHardwareController*>(hardwareControllerAtomic.load());

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
        [&hardwareControllerAtomic, mainLogger, userLogger](AutomaticWateringCommand::option_parser::const_option_pointer option) {
            auto valueOption = std::static_pointer_cast<
                const gh_cmd::Value<CharType, rpi_gc::automatic_watering::DailyCycleAWSHardwareController::digital_output_id>>(option);

            assert(static_cast<bool>(valueOption));
            auto* const hardwareController = static_cast<rpi_gc::automatic_watering::DailyCycleAWSHardwareController*>(hardwareControllerAtomic.load());

            hardwareController->setWaterPumpDigitalOutputID(valueOption->value());

            std::ostringstream formatString{};
            formatString << "Received new pump pin ID: ";
            formatString << valueOption->value();

            userLogger->logWarning(formatString.str());
            mainLogger->logWarning(formatString.str());
        }
    );

    auto abortCommand = std::make_unique<commands::AbortCommand>(mainLogger,
        std::vector<commands::AbortCommand::emergency_stoppable_system_pointer>{automaticWateringSystem}
    );

    auto helpCommand = std::make_unique<HelpCommand>(
        std::cout,
        std::vector<TerminalCommandType*>{
            versionCommand.get(),
            autoWateringCommand.get(),
            abortCommand.get()
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
