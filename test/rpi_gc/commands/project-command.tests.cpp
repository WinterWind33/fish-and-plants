// Copyright (c) 2023 Andrea Ballestrazzi
#include <commands/project-command.hpp>
#include <user-interface/commands-strings.hpp>

// Test doubles
#include <gh_cmd/test-doubles/option-parser.mock.hpp>
#include <gh_cmd/test-doubles/command-option.mock.hpp>

#include <testing-core.hpp>

// C++ STL
#include <memory>
#include <tuple>
#include <map>

TEST_CASE("ProjectCommand unit tests", "[unit][solitary][rpi_gc][commands][ProjectCommand]") {
    using namespace rpi_gc;

    SECTION("getName() should return the correct name") {
        auto dummyOptionParser{std::make_unique<testing::NiceMock<gh_cmd::mocks::OptionParserMock<char>>>()};
        const commands::ProjectCommand commandUnderTest{
            std::move(dummyOptionParser), commands::ProjectCommand::event_handler_map{}};

        CHECK(commandUnderTest.getName() == strings::commands::PROJECT);
    }

    SECTION("Behavioral tests") {
        GIVEN("A project command") {
            auto optionParserMock{std::make_unique<testing::StrictMock<gh_cmd::mocks::OptionParserMock<char>>>()};
            auto& optionParserMockRef{*optionParserMock};

            commands::ProjectCommand commandUnderTest{std::move(optionParserMock), commands::ProjectCommand::event_handler_map{}};

            WHEN("input options are processed") {
                const std::vector<commands::ProjectCommand::string_type> inputOptions{"--help", "-h"};

                THEN("The processing should be delegated to the option parser") {
                    EXPECT_CALL(optionParserMockRef, parse(inputOptions))
                        .Times(1);

                    [[maybe_unused]] const bool bRes{commandUnderTest.processInputOptions(inputOptions)};
                }
            }


        }
    }

    SECTION("State tests") {
        auto dummyOptionParser{std::make_unique<testing::NiceMock<gh_cmd::mocks::OptionParserMock<char>>>()};
        auto& optionParserStub{*dummyOptionParser};
        std::map<std::string, bool> callsPool{
            {"test", false}
        };
        commands::ProjectCommand::event_handler_map eventHandlerMap{
            {
                "test",
                [&callsPool](
                    [[maybe_unused]] commands::ProjectCommand::option_parser::const_option_pointer optionPointer){
                    callsPool["test"] = true;
                }
            }
        };

        commands::ProjectCommand commandUnderTest{std::move(dummyOptionParser), std::move(eventHandlerMap)};

        SECTION("processInputOptions() should always return true") {
            const std::vector<commands::ProjectCommand::string_type> inputOptions{"--help", "junk"};
            CHECK(commandUnderTest.processInputOptions(inputOptions));
        }

        WHEN("The command is executed with a valid option") {
            auto testOptionMock{std::make_shared<testing::NiceMock<gh_cmd::mocks::CommandOptionMock<char>>>()};
            ON_CALL(*testOptionMock, getLongName).WillByDefault(testing::Return("test"));
            ON_CALL(*testOptionMock, isSet).WillByDefault(testing::Return(true));

            ON_CALL(optionParserStub, getOptions()).WillByDefault(
                testing::Return(std::vector<commands::ProjectCommand::option_parser::option_pointer>{testOptionMock})
            );
            const bool bRes{commandUnderTest.execute()};

            THEN("It should call the correct event handler") {
                CHECK(callsPool["test"]);
            }

            THEN("The execution should succeed") {
                CHECK(bRes);
            }
        }
    }
}
