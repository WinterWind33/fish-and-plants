// Copyright (c) 2023 Andrea Ballestrazzi
#include <testing-core.hpp>

#include <user-interface/application-strings.hpp>

#include <commands/application-command.hpp>
#include <commands/help-command.hpp>
#include <commands/version-command.hpp>
#include <gh_cmd/gh_cmd.hpp>

TEST_CASE("Bivalent commands and ApplicationCommand integration",
          "[integration][ApplicationCommand][HelpCommand][VersionCommand]") {
    using namespace rpi_gc;

    OutputStringStream outputStream{};

    SECTION("ApplicationCommand execution") {
        gh_cmd::DefaultOptionParser<CharType> optionParser{};

        VersionCommand versionCommand{outputStream};
        HelpCommand helpCommand{
            outputStream, std::vector<HelpCommand::terminal_command_const_ref>{versionCommand}};

        ApplicationCommand commandUnderTest{outputStream, optionParser};
        commandUnderTest.addBivalentCommand(versionCommand);
        commandUnderTest.addBivalentCommand(helpCommand);

        SECTION("Executing version command should halt the execution flow") {
            commandUnderTest.processInputOptions(
                {StringType{strings::application::EXECUTABLE_NAME}, "--version"});

            const bool bRes{commandUnderTest.execute()};
            CHECK_FALSE(bRes);
        }

        SECTION("Executing version command should halt the execution flow") {
            commandUnderTest.processInputOptions(
                {StringType{strings::application::EXECUTABLE_NAME}, "--help"});

            const bool bRes{commandUnderTest.execute()};
            CHECK_FALSE(bRes);
        }
    }
}
