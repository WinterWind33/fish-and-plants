// Copyright (C) 2022 Andrea Ballestrazzi
#include <greenhouse-controller-application.hpp>

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

    OutputStringStream applicationHelpStream{};

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

    GreenhouseControllerApplication mainApplication{std::cout, std::cin};
    mainApplication.addSupportedCommand(std::move(autoWateringCommand));
    mainApplication.addSupportedCommand(std::move(versionCommand));
    mainApplication.addSupportedCommand(std::move(helpCommand));
    mainApplication.setApplicationCommand(std::move(applicationCommand));

    if(!mainApplication.processInputOptions(argc, argv))
        return 1;

    mainApplication.run();

    return 0;
}
