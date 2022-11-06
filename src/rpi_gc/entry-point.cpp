// Copyright (C) 2022 Andrea Ballestrazzi
#include <greenhouse-controller-application.hpp>

#include <gh_cmd/gh_cmd.hpp>

// C++ STL
#include <iostream>

// This is the entry point of the application. Here, it starts
// the main execution of the greenhouse controller.
int main(int argc, char* argv[]) {
    using namespace rpi_gc;

    // We pass the standard output as the output stream and the standard input as the input stream for now.
    // This may change in the future.
    using TerminalOptionParser = gh_cmd::DefaultOptionParser<CharType>;
    std::unique_ptr<TerminalOptionParser> terminalOptionParser{std::make_unique<TerminalOptionParser>("Terminal option parser")};
    terminalOptionParser->addSwitch(std::make_shared<gh_cmd::Switch<CharType>>('h', "--help", "Prints the help page (does not run the app)."));
    terminalOptionParser->addSwitch(std::make_shared<gh_cmd::Switch<CharType>>('v', "--version", "Displays the version of the application (does not run the app)."));

    GreenhouseControllerApplication mainApplication{std::cout, std::cin, std::move(terminalOptionParser)};

    if(!mainApplication.processInputOptions(argc, argv))
        return 0;

    mainApplication.run();

    return 0;
}
