// Copyright (c) 2022 Andrea Ballestrazzi
#include <testing-core.hpp>

#include <commands/automatic-watering/automatic-watering-command.hpp>
#include <user-interface/application-strings.hpp>

// Test Doubles
#include <gh_cmd/test-doubles/option-parser.mock.hpp>
#include <gh_cmd/test-doubles/command-option.mock.hpp>

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
            using CommandOptionMock = testing::NiceMock<gh_cmd::mocks::CommandOptionMock<CharType>>;
            std::shared_ptr<CommandOptionMock> optionMock{std::make_shared<CommandOptionMock>()};
            ON_CALL(*optionMock, getLongName).WillByDefault(testing::Return(OPTION_NAME));
            ON_CALL(*optionMock, isSet).WillByDefault(testing::Return(true));

            EXPECT_CALL(*optionParserRef, getOptions)
                .Times(1)
                .WillOnce(testing::Return(std::vector<AutomaticWateringCommand::option_parser::const_option_pointer>{
                    optionMock
                }));

            commandUnderTest.execute();

            THEN("The command should trigger the event") {
                CHECK(bEventCalled);
            }
        }

        AND_WHEN("The command is parsed and executed with the option and the \'help\' option") {
            using CommandOptionMock = testing::NiceMock<gh_cmd::mocks::CommandOptionMock<CharType>>;

            std::shared_ptr<CommandOptionMock> optionMock{std::make_shared<CommandOptionMock>()};
            ON_CALL(*optionMock, getLongName).WillByDefault(testing::Return(OPTION_NAME));
            ON_CALL(*optionMock, isSet).WillByDefault(testing::Return(true));

            std::shared_ptr<CommandOptionMock> helpOptionMock{std::make_shared<CommandOptionMock>()};
            ON_CALL(*helpOptionMock, getLongName).WillByDefault(testing::Return("help"));
            ON_CALL(*helpOptionMock, isSet).WillByDefault(testing::Return(true));

            EXPECT_CALL(*optionParserRef, getOptions)
                .Times(1)
                .WillOnce(testing::Return(std::vector<AutomaticWateringCommand::option_parser::const_option_pointer>{
                    optionMock,
                    helpOptionMock
                }));
            EXPECT_CALL(*optionParserRef, printHelp).Times(1);

            commandUnderTest.execute();

            THEN("The command should NOT trigger the event as help is executed first") {
                CHECK_FALSE(bEventCalled);
            }
        }
    }

}
