// Copyright (c) 2023 Andrea Ballestrazzi
#include <testing-core.hpp>

#include <commands/abort-command.hpp>

// Test Doubles
#include <gh_log/test-doubles/logger.mock.hpp>
#include <rpi_gc/test-doubles/abort-system/emergency-stoppable-system.mock.hpp>
#include <gh_cmd/test-doubles/option-parser.mock.hpp>

// C++ STL
#include <memory>

TEST_CASE("AbortCommand execution unit tests", "[unit][solitary][rpi_gc][commands][AbortCommand][execution]") {
    using rpi_gc::commands::AbortCommand;
    using testing::NiceMock;
    using testing::StrictMock;

    GIVEN("An abort command with one emergency stoppable system") {
        auto stoppableSystemMock = std::make_shared<StrictMock<rpi_gc::abort_system::mocks::EmergencyStoppableSystemMock>>();
        auto loggerMock = std::make_shared<NiceMock<gh_log::mocks::LoggerMock>>();
        auto optionParserMock{std::make_unique<NiceMock<gh_cmd::mocks::OptionParserMock<char>>>()};
        auto& optionParserMockRef{*optionParserMock};

        AbortCommand commandUnderTest{loggerMock, {stoppableSystemMock}, std::move(optionParserMock)};

        WHEN("execute() is called") {
            THEN("emergencyAbort() should be called on the stoppable system") {
                ON_CALL(optionParserMockRef, getOptions()).WillByDefault(testing::Return(std::vector<AbortCommand::option_parser::option_pointer>{}));
                EXPECT_CALL(*stoppableSystemMock, emergencyAbort).Times(1);

                commandUnderTest.execute();
            }
        }
    }
}
