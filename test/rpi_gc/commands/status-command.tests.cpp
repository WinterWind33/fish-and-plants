// Copyright (c) 2023 Andrea Ballestrazzi

#include <commands/status-command.hpp>

// Test doubles
#include <gh_cmd/test-doubles/command-option.mock.hpp>
#include <gh_cmd/test-doubles/option-parser.mock.hpp>
#include <rpi_gc/test-doubles/diagnostics/diagnostic-status-probeable.mock.hpp>

#include <testing-core.hpp>

// C++ STL
#include <sstream>
#include <tuple>

namespace tests {

template <typename MockType>
auto createOptionParserMock() noexcept {
    auto mockSmartPtr = std::make_unique<MockType>();
    return std::make_pair(mockSmartPtr.get(), std::move(mockSmartPtr));
}

} // namespace tests

TEST_CASE("Status command unit tests", "[unit][solitary][rpi_gc][commands][StatusCommand]") {
    using namespace rpi_gc;
    using OptionParserMockType = gh_cmd::mocks::OptionParserMock<char>;

    std::ostringstream dummyOutputStream{};

    SECTION("Command methods - no execute()") {
        auto [optionParserMock, optionParserMockSmartPtr]{
            tests::createOptionParserMock<testing::StrictMock<OptionParserMockType>>()};

        commands::StatusCommand commandUnderTest{
            std::move(optionParserMockSmartPtr), {}, std::ref(dummyOutputStream)};

        SECTION("getName() method should return the correct name") {
            CHECK(commandUnderTest.getName() == strings::commands::STATUS);
        }

        WHEN("Printing the help page") {
            THEN("It should call the option parser printHelp() one time") {
                EXPECT_CALL(*optionParserMock, printHelp(testing::Ref(dummyOutputStream))).Times(1);

                commandUnderTest.printHelp(std::ref(dummyOutputStream));
            }
        }

        WHEN("Processing the input options") {
            THEN("It should call the option parser to parse the arguments") {
                EXPECT_CALL(*optionParserMock, parse(testing::_)).Times(1);

                [[maybe_unused]] const bool bExec{commandUnderTest.processInputOptions({"--help"})};
            }

            THEN("It should call the option parser with the correct arguments") {
                const std::vector<std::string> testArguments{"-h", "--help"};

                EXPECT_CALL(*optionParserMock, parse(testArguments));

                [[maybe_unused]] const bool bExec{
                    commandUnderTest.processInputOptions(testArguments)};
            }

            THEN("It should return a successful state to continue the execution") {
                EXPECT_CALL(*optionParserMock, parse(testing::_));

                const bool bExec{commandUnderTest.processInputOptions({})};
                CHECK(bExec);
            }
        }
    }

    SECTION("Command execute() method") {
        using DiagnosticObjectMock =
            testing::StrictMock<diagnostics::mocks::DiagnosticStatusProbeableMock>;
        using HelpOptionMock = testing::StrictMock<gh_cmd::mocks::CommandOptionMock<char>>;

        GIVEN("A StatusCommand object with one diagnostic probeable object") {
            auto [optionParserMock, optionParserMockSmartPtr]{
                tests::createOptionParserMock<testing::NiceMock<OptionParserMockType>>()};

            DiagnosticObjectMock diagnosticMock{};

            commands::StatusCommand commandUnderTest{std::move(optionParserMockSmartPtr),
                                                     {std::cref(diagnosticMock)},
                                                     std::ref(dummyOutputStream)};

            std::vector<OptionParserMockType::option_pointer> optionPointers{};
            WHEN("An help option is passed as an argument to the command") {
                auto helpOptionMock = std::make_shared<HelpOptionMock>();
                EXPECT_CALL(*helpOptionMock, getLongName)
                    .Times(testing::AtLeast(1))
                    .WillRepeatedly(testing::Return("help"));

                EXPECT_CALL(*helpOptionMock, isSet).Times(1).WillOnce(testing::Return(true));

                EXPECT_CALL(*helpOptionMock, clear).Times(1);

                optionPointers.push_back(helpOptionMock);
                ON_CALL(*optionParserMock, getOptions())
                    .WillByDefault(testing::Return(optionPointers));

                THEN("It should print the help page without printing any diagnostics") {
                    EXPECT_CALL(diagnosticMock, printDiagnostic(testing::Ref(dummyOutputStream)))
                        .Times(0);

                    [[maybe_unused]] const bool bExec{commandUnderTest.execute()};
                }

                THEN("The execution should succeed") {
                    const bool bExec{commandUnderTest.execute()};

                    CHECK(bExec);
                }
            }

            WHEN("The command is passed without any arguments") {
                THEN("It should be printed the diagnosticable object status") {
                    EXPECT_CALL(diagnosticMock, printDiagnostic(testing::Ref(dummyOutputStream)))
                        .Times(1);

                    [[maybe_unused]] const bool bExec{commandUnderTest.execute()};
                }

                THEN("The execution should succeed") {
                    EXPECT_CALL(diagnosticMock, printDiagnostic(testing::_));

                    const bool bExec{commandUnderTest.execute()};
                    CHECK(bExec);
                }
            }
        }
    }
}
