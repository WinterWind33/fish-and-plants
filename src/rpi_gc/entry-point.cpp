// Copyright (C) 2022 Andrea Ballestrazzi
#include <greenhouse-controller-application.hpp>

// Commands
#include <gh_cmd/gh_cmd.hpp>
#include <commands/application-command.hpp>
#include <commands/version-command.hpp>
#include <commands/help-command.hpp>

// C++ STL
#include <iostream>
#include <memory>
#include <algorithm>

// This is the entry point of the application. Here, it starts
// the main execution of the greenhouse controller.
int main(int argc, char* argv[]) {
    using namespace rpi_gc;

    // We pass the standard output as the output stream and the standard input as the input stream for now.
    // This may change in the future.
    using TerminalOptionParser = gh_cmd::DefaultOptionParser<CharType>;
    std::unique_ptr<TerminalOptionParser> terminalOptionParser{std::make_unique<TerminalOptionParser>("rpi_gc options")};
    terminalOptionParser->addSwitch(std::make_shared<gh_cmd::Switch<CharType>>('h', "help", "Prints the help page (does not run the app)."));
    terminalOptionParser->addSwitch(std::make_shared<gh_cmd::Switch<CharType>>('v', "version", "Displays the version of the application (does not run the app)."));

    std::unique_ptr<ApplicationCommand> applicationCommand{std::make_unique<ApplicationCommand>(std::cout, *terminalOptionParser)};

    GreenhouseControllerApplication mainApplication{std::cout, std::cin, std::move(terminalOptionParser)};
    mainApplication.setApplicationCommand(std::move(applicationCommand));

    std::unique_ptr<TerminalOptionParser> versionParser{std::make_unique<TerminalOptionParser>("version command - displays the software version")};

    HelpCommand::option_parsers_map optionParsers{};
    optionParsers.emplace("version", *versionParser);

    mainApplication.addSupportedCommand(std::make_unique<VersionCommand>(std::cout), std::move(versionParser));

    std::unique_ptr<TerminalOptionParser> helpParser{std::make_unique<TerminalOptionParser>("help command - displays this help page")};
    optionParsers.emplace("help", *helpParser);

    mainApplication.addSupportedCommand(std::make_unique<HelpCommand>(std::cout, std::move(optionParsers)), std::move(helpParser));

    if(!mainApplication.processInputOptions(argc, argv))
        return 0;

    mainApplication.run();

    return 0;
}
