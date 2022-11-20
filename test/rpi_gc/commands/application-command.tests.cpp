// Copyright (c) 2022 Andrea Ballestrazzi
#include <testing-core.hpp>

#include <commands/application-command.hpp>
#include <user-interface/application-strings.hpp>

// Test Doubles
#include <gh_cmd/test-doubles/option-parser.mock.hpp>
#include <gh_cmd/test-doubles/command-option.mock.hpp>
#include <rpi_gc/test-doubles/commands/bivalent-command.mock.hpp>

TEST_CASE("ApplicationCommand option parsing unit tests", "[unit][solitary][rpi_gc][ApplicationCommand][parsing]") {
    using namespace rpi_gc;
    using OptionParserMock = gh_cmd::mocks::OptionParserMock<CharType>;

    OutputStringStream outputStream{};

    GIVEN("An application command") {
        using testing::StrictMock;

        StrictMock<OptionParserMock> optionParserMock{};
        ApplicationCommand commandUnderTest{outputStream, optionParserMock};

        AND_GIVEN("A generic and valid input line") {
            std::vector<ApplicationCommand::string_type> inputLine{
                StringType{strings::application::EXECUTABLE_NAME},
                "--option",
                "--",
                "test"};
            WHEN("the input line is processed by the command") {
                THEN("The options should be parsed by the option parser") {
                    EXPECT_CALL(optionParserMock, parse(inputLine)).Times(1);

                    commandUnderTest.processInputOptions(inputLine);
                }

                THEN("The method should not block the command execution") {
                    EXPECT_CALL(optionParserMock, parse(inputLine)).Times(1);

                    const bool bRes{commandUnderTest.processInputOptions(inputLine)};
                    CHECK(bRes);
                }
            }
        }
    }
}

TEST_CASE("ApplicationCommand generic unit tests", "[unit][solitary][rpi_gc][ApplicationCommand][generic]") {
    using namespace rpi_gc;
    using BivalentCommandMock = rpi_gc::mocks::BivalentCommandMock<CharType>;
    using OptionParserMock = gh_cmd::mocks::OptionParserMock<CharType>;
    using CommandOptionMock = gh_cmd::mocks::CommandOptionMock<CharType>;

    OutputStringStream outputStream{};

    SECTION("addBivalentCommand() method") {
        using testing::StrictMock;
        using testing::NiceMock;

        SECTION("Should query the command as option") {
            // In this test we don't care about the option parser.
            NiceMock<OptionParserMock> optionParserMock{};
            std::shared_ptr<NiceMock<CommandOptionMock>> commandOptionMock{std::make_shared<NiceMock<CommandOptionMock>>()};
            ON_CALL(*commandOptionMock, getLongName).WillByDefault(testing::Return(StringType{strings::commands::HELP}));

            StrictMock<BivalentCommandMock> bivalentCommandMock{};
            EXPECT_CALL(bivalentCommandMock, getAsOption)
                .Times(1)
                .WillOnce(testing::Return(commandOptionMock));

            ApplicationCommand commandUnderTest{outputStream, optionParserMock};
            commandUnderTest.addBivalentCommand(bivalentCommandMock);
        }

        SECTION("Should add the command as option to the option parser") {
            std::shared_ptr<NiceMock<CommandOptionMock>> commandOptionMock{std::make_shared<NiceMock<CommandOptionMock>>()};
            ON_CALL(*commandOptionMock, getLongName).WillByDefault(testing::Return(StringType{strings::commands::HELP}));

            NiceMock<BivalentCommandMock> bivalentCommandMock{};
            ON_CALL(bivalentCommandMock, getAsOption).WillByDefault(testing::Return(commandOptionMock));

            StrictMock<OptionParserMock> optionParserMock{};
            EXPECT_CALL(optionParserMock, addOption(std::static_pointer_cast<gh_cmd::CommandOption<CharType>>(commandOptionMock)))
                .Times(1);

            ApplicationCommand commandUnderTest{outputStream, optionParserMock};
            commandUnderTest.addBivalentCommand(bivalentCommandMock);
        }
    }
}

TEST_CASE("ApplicationCommand execution unit tests", "[unit][solitary][rpi_gc][ApplicationCommand][execution]") {

}
