// Copyright (c) 2023 Andrea Ballestrazzi
#include <testing-core.hpp>

#include <commands/automatic-watering/automatic-watering-command.hpp>
#include <user-interface/application-strings.hpp>

// Test Doubles
#include <gh_cmd/test-doubles/option-parser.mock.hpp>
#include <gh_cmd/test-doubles/command-option.mock.hpp>

namespace tests {

    template<typename MockType>
    auto ConfigureCommandOptionMock(auto&& longOptionName, const bool bIsSet) noexcept {
        auto commandStubPointer = std::make_shared<MockType>();
        ON_CALL(*commandStubPointer, getLongName).WillByDefault(testing::Return(longOptionName));
        ON_CALL(*commandStubPointer, isSet).WillByDefault(testing::Return(bIsSet));

        return commandStubPointer;
    }

} // namespace tests

TEST_CASE("AutomaticWateringCommand Events unit tests", "[unit][solitary][rpi_gc][AutomaticWateringCommand][events]") {
    using namespace rpi_gc;
    using testing::StrictMock;

    using OptionParserMock = StrictMock<gh_cmd::mocks::OptionParserMock<CharType>>;

    OutputStringStream outputStream{};
    std::unique_ptr<OptionParserMock> optionParserMock{std::make_unique<OptionParserMock>()};
    OptionParserMock* optionParserRef{optionParserMock.get()};

    AutomaticWateringCommand commandUnderTest{outputStream, std::move(optionParserMock)};

    WHEN("An event is added to the command") {
        const AutomaticWateringCommand::option_type::long_name_type OPTION_NAME{"test-option"};
        bool bEventCalled{false};

        auto eventMock = [&bEventCalled, OPTION_NAME](AutomaticWateringCommand::option_parser::const_option_pointer optionPointer){
            REQUIRE(optionPointer != nullptr);

            CHECK(optionPointer->getLongName() == OPTION_NAME);
            bEventCalled = true;
        };

        commandUnderTest.registerOptionEvent(OPTION_NAME, eventMock);

        AND_WHEN("The command is parsed and executed with the option") {
            using CommandOptionStub = testing::NiceMock<gh_cmd::mocks::CommandOptionMock<CharType>>;
            auto optionMock{tests::ConfigureCommandOptionMock<CommandOptionStub>(OPTION_NAME, true)};

            EXPECT_CALL(*optionParserRef, getOptions())
                .Times(1)
                .WillOnce(testing::Return(std::vector<AutomaticWateringCommand::option_parser::option_pointer>{
                    optionMock
                }));

            commandUnderTest.execute();

            THEN("The command should trigger the event") {
                CHECK(bEventCalled);
            }
        }

        AND_WHEN("The command is parsed and executed with the option and the \'help\' option") {
            using CommandOptionStub = testing::NiceMock<gh_cmd::mocks::CommandOptionMock<CharType>>;

            auto optionMock{tests::ConfigureCommandOptionMock<CommandOptionStub>(OPTION_NAME, true)};
            auto helpOptionMock{tests::ConfigureCommandOptionMock<CommandOptionStub>(strings::commands::options::AUTOMATIC_WATERING_HELP.data(), true)};

            EXPECT_CALL(*optionParserRef, getOptions())
                .Times(1)
                .WillOnce(testing::Return(std::vector<AutomaticWateringCommand::option_parser::option_pointer>{
                    optionMock,
                    helpOptionMock
                }));
            EXPECT_CALL(*optionParserRef, printHelp).Times(1);

            commandUnderTest.execute();

            THEN("The command should NOT trigger the event as help is executed first") {
                CHECK_FALSE(bEventCalled);
            }
        }

        AND_WHEN("The command is parsed and executed with the option and the \'stop\' option") {
            using CommandOptionStub = testing::NiceMock<gh_cmd::mocks::CommandOptionMock<CharType>>;

            auto optionStub{tests::ConfigureCommandOptionMock<CommandOptionStub>(OPTION_NAME, true)};
            auto stopOptionStub{tests::ConfigureCommandOptionMock<CommandOptionStub>(strings::commands::options::AUTOMATIC_WATERING_STOP.data(), true)};

            EXPECT_CALL(*optionParserRef, getOptions())
                .Times(1)
                .WillOnce(testing::Return(std::vector<AutomaticWateringCommand::option_parser::option_pointer>{
                    optionStub,
                    stopOptionStub
                }));

            commandUnderTest.execute();
            THEN("The command should NOT trigger the event as stop is executed first") {
                CHECK_FALSE(bEventCalled);
            }
        }
    }
}
