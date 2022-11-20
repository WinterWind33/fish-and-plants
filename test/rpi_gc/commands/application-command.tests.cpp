// Copyright (c) 2022 Andrea Ballestrazzi
#include <testing-core.hpp>

#include <commands/application-command.hpp>
#include <user-interface/application-strings.hpp>

// Test Doubles
#include <gh_cmd/test-doubles/option-parser.mock.hpp>
#include <rpi_gc/test-doubles/commands/bivalent-command.mock.hpp>

TEST_CASE("ApplicationCommand option parsing unit tests", "[unit][solitary][rpi_gc][ApplicationCommand]") {
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
