// Copyright (C) 2023 Andrea Ballestrazzi
#include <greenhouse-controller-application.hpp>

#include <automatic-watering/daily-cycle-automatic-watering-system.hpp>
#include <automatic-watering/hardware-controllers/daily-cycle-aws-hardware-controller.hpp>
#include <automatic-watering/time-providers/daily-cycle-aws-time-provider.hpp>
#include <gh_log/logger.hpp>
#include <gh_log/spl-logger.hpp>

// Commands
#include <gh_cmd/gh_cmd.hpp>
#include <commands/application-command.hpp>
#include <commands/abort-command.hpp>
#include <commands/version-command.hpp>
#include <commands/help-command.hpp>
#include <commands/automatic-watering/automatic-watering-command.hpp>

// C++ STL
#include <iostream>
#include <memory>
#include <algorithm>
#include <atomic>

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
            std::make_shared<gh_cmd::Value<CharType, std::int64_t>>(
                'A', "activation-time", "Sets the automatic watering system activation time (expressed in ms).", defaultTimeProvider.getWateringSystemActivationDuration().count()));
        autoWateringOptionParser->addOption(
            std::make_shared<gh_cmd::Value<CharType, std::int64_t>>(
                'D', "deactivation-time", "Sets the automatic watering system deactivation time (expressed in ms).", defaultTimeProvider.getWateringSystemDeactivationDuration().count()));

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

namespace constants {

    constexpr rpi_gc::automatic_watering::DailyCycleAWSHardwareController::digital_output_id WATER_VALVE_PIN_ID{26};
    constexpr rpi_gc::automatic_watering::DailyCycleAWSHardwareController::digital_output_id WATER_PUMP_PIN_ID{23};

} // namespace constants

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

    auto awsTimeProviderSmartPtr{std::make_shared<rpi_gc::automatic_watering::DailyCycleAWSTimeProvider>()};

    rpi_gc::automatic_watering::DailyCycleAutomaticWateringSystem::time_provider_pointer awsTimeProvider{
        awsTimeProviderSmartPtr.get()
    };

    using AutomaticWateringSystemPointer = std::shared_ptr<automatic_watering::DailyCycleAutomaticWateringSystem>;
    AutomaticWateringSystemPointer automaticWateringSystem{
        std::make_shared<automatic_watering::DailyCycleAutomaticWateringSystem>(
            mainLogger,
            userLogger,
            std::make_unique<rpi_gc::automatic_watering::DailyCycleAWSHardwareController>(constants::WATER_VALVE_PIN_ID, constants::WATER_PUMP_PIN_ID),
            std::ref(awsTimeProvider)
        )
    };

    mainLogger->logInfo("Initiating application commands and user interface...");
    auto versionCommand = std::make_unique<VersionCommand>(std::cout);

    auto autoWateringCommand = commands_factory::CreateAutomaticWateringCommand<AutomaticWateringSystemPointer, DefaultOptionParser>(
            automaticWateringSystem
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
