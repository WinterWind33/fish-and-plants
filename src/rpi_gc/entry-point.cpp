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

// This is the entry point of the application. Here, it starts
// the main execution of the greenhouse controller.
int main(int argc, char* argv[]) {
    using namespace rpi_gc;

    using DefaultOptionParser = gh_cmd::DefaultOptionParser<CharType>;
    using ApplicationOptionParser = DefaultOptionParser;

    std::shared_ptr<gh_log::Logger> mainLogger{gh_log::SPLLogger::createFileLogger(
        StringType{strings::application::NAME},
        strings::application::MAIN_LOG_FILENAME
    )};

    mainLogger->logInfo("Initiating system: starting log now.");

    OutputStringStream applicationHelpStream{};
    std::shared_ptr<automatic_watering::DailyCycleAutomaticWateringSystem> automaticWateringSystem{
        std::make_shared<automatic_watering::DailyCycleAutomaticWateringSystem>(
            mainLogger
        )
    };

    mainLogger->logInfo("Initiating application commands and user interface...");
    AutomaticWateringCommand::option_parser_pointer optionParser{std::make_unique<DefaultOptionParser>("[OPTIONS] => auto-watering")};
    optionParser->addSwitch(std::make_shared<gh_cmd::Switch<CharType>>('h', "help", "Displays this help page."));

    std::unique_ptr<VersionCommand> versionCommand{std::make_unique<VersionCommand>(std::cout)};
    std::unique_ptr<AutomaticWateringCommand> autoWateringCommand{std::make_unique<AutomaticWateringCommand>(std::cout, std::move(optionParser))};
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
