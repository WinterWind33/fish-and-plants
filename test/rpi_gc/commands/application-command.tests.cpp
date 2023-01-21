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
    using namespace rpi_gc;
    using testing::NiceMock;
    using testing::StrictMock;

    using OptionParserMock = NiceMock<gh_cmd::mocks::OptionParserMock<CharType>>;
    using BivalentCommandMock = mocks::BivalentCommandMock<CharType>;
    using CommandOptionMock = gh_cmd::mocks::CommandOptionMock<CharType>;

    OutputStringStream outputStream{};
    OptionParserMock optionParser{};
    ApplicationCommand commandUnderTest{outputStream, optionParser};

    GIVEN("An input line with a valid option") {
        using OptionPointer = std::shared_ptr<gh_cmd::CommandOption<CharType>>;

        std::vector<ApplicationCommand::string_type> inputLine{
            StringType{strings::application::EXECUTABLE_NAME},
            "--version"
        };

        std::shared_ptr<NiceMock<CommandOptionMock>> commandOptionMock{std::make_shared<NiceMock<CommandOptionMock>>()};
        ON_CALL(*commandOptionMock, getLongName).WillByDefault(testing::Return(StringType{strings::commands::VERSION}));
        ON_CALL(*commandOptionMock, value).WillByDefault(testing::Return(true));

        ON_CALL(optionParser, getOptions()).WillByDefault(testing::Return(std::vector<OptionPointer>{commandOptionMock}));

        StrictMock<BivalentCommandMock> commandMock{};
        EXPECT_CALL(commandMock, getAsOption).WillRepeatedly(testing::Return(commandOptionMock));

        commandUnderTest.addBivalentCommand(commandMock);
        commandUnderTest.processInputOptions(inputLine);

        WHEN("The command is executed") {
            THEN("It should be called the related command") {
                EXPECT_CALL(commandMock, executeAsOption)
                    .Times(1)
                    .WillOnce(testing::Return(true));

                commandUnderTest.execute();
            }

            THEN("It should return the same result as the option execution") {
                EXPECT_CALL(commandMock, executeAsOption)
                    .WillOnce(testing::Return(true))
                    .WillOnce(testing::Return(false));

                bool bRes{commandUnderTest.execute()};
                CHECK(bRes);

                bRes = commandUnderTest.execute();
                CHECK_FALSE(bRes);
            }
        }
    }

    GIVEN("An input line with two options and one is the help one.") {
        using OptionPointer = std::shared_ptr<gh_cmd::CommandOption<CharType>>;

        std::vector<ApplicationCommand::string_type> inputLine{
            StringType{strings::application::EXECUTABLE_NAME},
            "--help"
            "--version"
        };

        std::shared_ptr<NiceMock<CommandOptionMock>> versionCommandOptionMock{std::make_shared<NiceMock<CommandOptionMock>>()};
        ON_CALL(*versionCommandOptionMock, getLongName).WillByDefault(testing::Return(StringType{strings::commands::VERSION}));
        ON_CALL(*versionCommandOptionMock, value).WillByDefault(testing::Return(true));

        std::shared_ptr<NiceMock<CommandOptionMock>> helpCommandOptionMock{std::make_shared<NiceMock<CommandOptionMock>>()};
        ON_CALL(*helpCommandOptionMock, getLongName).WillByDefault(testing::Return(StringType{strings::commands::HELP}));
        ON_CALL(*helpCommandOptionMock, value).WillByDefault(testing::Return(true));

        ON_CALL(optionParser, getOptions()).WillByDefault(testing::Return(std::vector<OptionPointer>{versionCommandOptionMock, helpCommandOptionMock}));

        StrictMock<BivalentCommandMock> versionCommandMock{};
        EXPECT_CALL(versionCommandMock, getAsOption).WillRepeatedly(testing::Return(versionCommandOptionMock));

        StrictMock<BivalentCommandMock> helpCommandMock{};
        EXPECT_CALL(helpCommandMock, getAsOption).WillRepeatedly(testing::Return(helpCommandOptionMock));

        commandUnderTest.addBivalentCommand(versionCommandMock);
        commandUnderTest.addBivalentCommand(helpCommandMock);

        commandUnderTest.processInputOptions(inputLine);

        WHEN("The command is executed") {
            THEN("It should be executed only the help command as option") {
                EXPECT_CALL(versionCommandMock, executeAsOption).Times(0);
                EXPECT_CALL(helpCommandMock, executeAsOption)
                    .Times(1)
                    .WillOnce(testing::Return(false));

                commandUnderTest.execute();
            }
        }
    }
}
