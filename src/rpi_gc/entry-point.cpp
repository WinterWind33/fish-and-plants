// Copyright (C) 2022 Andrea Ballestrazzi
#include <greenhouse-controller-application.hpp>

#include <automatic-watering/daily-cycle-automatic-watering-system.hpp>
#include <gh_log/logger.hpp>
#include <gh_log/spl-logger.hpp>

// Commands
#include <gh_cmd/gh_cmd.hpp>
#include <commands/application-command.hpp>
#include <commands/version-command.hpp>
#include <commands/help-command.hpp>
#include <commands/automatic-watering/automatic-watering-command.hpp>

// C++ STL
#include <iostream>
#include <memory>
#include <algorithm>

namespace commands_factory {

    template<typename WateringSystemPointer, typename OptionParserType>
    [[nodiscard]] static std::unique_ptr<rpi_gc::AutomaticWateringCommand>
        CreateAutomaticWateringCommand(WateringSystemPointer wateringSystem) {
        using rpi_gc::AutomaticWateringCommand;
        using rpi_gc::CharType;

        assert((bool)wateringSystem);

        AutomaticWateringCommand::option_parser_pointer autoWateringOptionParser{std::make_unique<OptionParserType>("[OPTIONS] => auto-watering")};
        autoWateringOptionParser->addSwitch(std::make_shared<gh_cmd::Switch<CharType>>('h', "help", "Displays this help page."));
        autoWateringOptionParser->addSwitch(std::make_shared<gh_cmd::Switch<CharType>>('S', "start", "Starts the automatic watering system in Daily-Cycle mode."));

        std::unique_ptr<AutomaticWateringCommand> autoWateringCommand{std::make_unique<AutomaticWateringCommand>(std::cout, std::move(autoWateringOptionParser))};
        autoWateringCommand->registerOptionEvent(
            "start",
            [wateringSystem]([[maybe_unused]] AutomaticWateringCommand::option_parser::const_option_pointer) {
                wateringSystem->startAutomaticWatering();
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

    using AutomaticWateringSystemPointer = std::shared_ptr<automatic_watering::DailyCycleAutomaticWateringSystem>;
    AutomaticWateringSystemPointer automaticWateringSystem{
        std::make_shared<automatic_watering::DailyCycleAutomaticWateringSystem>(
            mainLogger,
            userLogger
        )
    };

    mainLogger->logInfo("Initiating application commands and user interface...");
    std::unique_ptr<VersionCommand> versionCommand{std::make_unique<VersionCommand>(std::cout)};

    std::unique_ptr<AutomaticWateringCommand> autoWateringCommand{
        commands_factory::CreateAutomaticWateringCommand<AutomaticWateringSystemPointer, DefaultOptionParser>(
            automaticWateringSystem
    )};

    std::unique_ptr<HelpCommand> helpCommand{std::make_unique<HelpCommand>(
        std::cout,
        std::vector<TerminalCommandType*>{
            versionCommand.get(),
            autoWateringCommand.get()
        }
    )};

    std::unique_ptr<ApplicationOptionParser> applicationOptionParser{std::make_unique<ApplicationOptionParser>("rpi_gc command options")};
    std::unique_ptr<ApplicationCommand> applicationCommand{std::make_unique<ApplicationCommand>(std::cout, *applicationOptionParser)};
    applicationCommand->addBivalentCommand(*versionCommand);
    applicationCommand->addBivalentCommand(*helpCommand);

    applicationOptionParser->printHelp(applicationHelpStream);
    helpCommand->setApplicationHelp(applicationHelpStream.str());

    mainLogger->logInfo("Initiating main application controller...");
    GreenhouseControllerApplication mainApplication{std::cout, std::cin};
    mainApplication.addSupportedCommand(std::move(autoWateringCommand));
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
