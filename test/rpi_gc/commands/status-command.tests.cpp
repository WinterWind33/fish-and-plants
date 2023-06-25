// Copyright (c) 2023 Andrea Ballestrazzi

#include <commands/status-command.hpp>

// Test doubles
#include <rpi_gc/test-doubles/diagnostics/diagnostic-status-probeable.mock.hpp>
#include <gh_cmd/test-doubles/option-parser.mock.hpp>

#include <testing-core.hpp>

// C++ STL
#include <tuple>
#include <ostream>

namespace tests {

    template<typename MockType>
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

        commands::StatusCommand commandUnderTest{std::move(optionParserMockSmartPtr), {}, std::ref(dummyOutputStream)};

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
                const std::vector<std::string> testArguments{
                    "-h",
                    "--help"
                };

                EXPECT_CALL(*optionParserMock, parse(testArguments));

                [[maybe_unused]] const bool bExec{commandUnderTest.processInputOptions(testArguments)};
            }
        }
    }
}
