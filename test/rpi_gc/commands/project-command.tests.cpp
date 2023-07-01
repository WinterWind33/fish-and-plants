// Copyright (c) 2023 Andrea Ballestrazzi
#include <commands/project-command.hpp>
#include <user-interface/commands-strings.hpp>

// Test doubles
#include <gh_cmd/test-doubles/option-parser.mock.hpp>

#include <testing-core.hpp>

// C++ STL
#include <memory>
#include <tuple>

TEST_CASE("ProjectCommand unit tests", "[unit][solitary][rpi_gc][commands][ProjectCommand]") {
    using namespace rpi_gc;

    SECTION("getName() should return the correct name") {
        auto dummyOptionParser{std::make_unique<testing::NiceMock<gh_cmd::mocks::OptionParserMock<char>>>()};
        const commands::ProjectCommand commandUnderTest{std::move(dummyOptionParser)};

        CHECK(commandUnderTest.getName() == strings::commands::PROJECT);
    }

    SECTION("Behavioral tests") {
        GIVEN("A project command") {
            auto optionParserMock{std::make_unique<testing::StrictMock<gh_cmd::mocks::OptionParserMock<char>>>()};
            auto& optionParserMockRef{*optionParserMock};
            commands::ProjectCommand commandUnderTest{std::move(optionParserMock)};

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

    SECTION("Data tests") {
        auto dummyOptionParser{std::make_unique<testing::NiceMock<gh_cmd::mocks::OptionParserMock<char>>>()};
        commands::ProjectCommand commandUnderTest{std::move(dummyOptionParser)};

        SECTION("processInputOptions() should always return true") {
            const std::vector<commands::ProjectCommand::string_type> inputOptions{"--help", "junk"};
            CHECK(commandUnderTest.processInputOptions(inputOptions));
        }
    }
}
