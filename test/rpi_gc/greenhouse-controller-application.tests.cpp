// Copyright (c) 2022 Andrea Ballestrazzi
#include <testing-core.hpp>

#include <greenhouse-controller-application.hpp>
#include <rpi_gc-config-file.hpp>

// Test doubles
#include <gh_cmd/test-doubles/option-parser.mock.hpp>

// C++ STL
#include <sstream>
#include <cstdint>

namespace tests {

    void VerifyLineEqual(const std::size_t lineNum, rpi_gc::OutputStringStream& actual, rpi_gc::StringType expected) {
        rpi_gc::StringType lineUnderTest{};
        rpi_gc::InputStringStream linesStream{actual.str()};
        for(std::size_t i{}; i < lineNum; ++i) {
            lineUnderTest.clear();
            REQUIRE(std::getline(linesStream, lineUnderTest).good());
        }

        CHECK(lineUnderTest == expected);
    }

    void VerifyLineEmpty(const std::size_t lineNum, rpi_gc::OutputStringStream& actual) {
        rpi_gc::StringType lineUnderTest{};
        rpi_gc::InputStringStream linesStream{actual.str()};
        for(std::size_t i{}; i < lineNum; ++i) {
            lineUnderTest.clear();
            REQUIRE(std::getline(linesStream, lineUnderTest).good());
        }

        CHECK(lineUnderTest.empty());
    }

    rpi_gc::StringType GenerateVersionString() noexcept {
        rpi_gc::OutputStringStream stream{};
        stream << rpi_gc_VERSION_MAJOR << "." << rpi_gc_VERSION_MINOR << "." << rpi_gc_VERSION_PATCH;

        return stream.str();
    }

} // namespace tests

TEST_CASE("Application Header Lines", "[functional][rpi_gc][GreenhouseControllerApplication][application-header]") {
    using namespace rpi_gc;

    rpi_gc::OutputStringStream outputStream{};
    rpi_gc::InputStringStream inputStream{"exit"};

    GreenhouseControllerApplication applicationUnderTest{outputStream, inputStream};

    SECTION("When running the application") {
        SECTION("It should correctly print the application name and version (first line)") {
            REQUIRE_NOTHROW(applicationUnderTest.run());

            tests::VerifyLineEqual(1, outputStream, rpi_gc::StringType{"Greenhouse Controller "} + tests::GenerateVersionString());
        }

        SECTION("It should correctly print the copyright disclaimer (second line)") {
            REQUIRE_NOTHROW(applicationUnderTest.run());

            tests::VerifyLineEqual(2, outputStream, "Copyright (c) 2022 Andrea Ballestrazzi");
        }

        SECTION("It should correctly print the team credit") {
            REQUIRE_NOTHROW(applicationUnderTest.run());

            tests::VerifyLineEqual(4, outputStream, "-- Fish&Plants Team --");
        }

        SECTION("It should add an end line between the disclaimer and the team credit") {
            REQUIRE_NOTHROW(applicationUnderTest.run());

            tests::VerifyLineEmpty(3, outputStream);
        }

        SECTION("It should add an end line after the team credit") {
            REQUIRE_NOTHROW(applicationUnderTest.run());

            tests::VerifyLineEmpty(5, outputStream);
        }
    }
}

TEST_CASE("Commands execution", "[unit][sociable][GreenhouseControllerApplication][commands]") {
    using namespace rpi_gc;

    GIVEN("An application controller with a command registered") {
        using testing::StrictMock;

        InputStringStream inputStream{};
        OutputStringStream outputStream{};
        GreenhouseControllerApplication applicationUnderTest{outputStream, inputStream};

        constexpr std::string_view COMMAND_NAME{"dummyName"};
        std::unique_ptr<StrictMock<gh_cmd::mocks::OptionParserMock<char>>> optionParserMockPtr{
            std::make_unique<StrictMock<gh_cmd::mocks::OptionParserMock<char>>>()};

        // We keep a reference to the mock so we can set the expectations later.
        gh_cmd::mocks::OptionParserMock<char>& optionParserMock{*optionParserMockPtr};

        applicationUnderTest.addSupportedCommand(StringType{COMMAND_NAME}, std::move(optionParserMockPtr));

        WHEN("The user executes the application and types the registered command (no options)") {
            // We mimic the user that types our command.
            inputStream.str(StringType{COMMAND_NAME});

            THEN("Its command parser should be called with the single command") {
                EXPECT_CALL(optionParserMock, parse(std::vector<StringType>{StringType{COMMAND_NAME}})).Times(1);
                applicationUnderTest.run();
            }
        }

        WHEN("The user executes the application and types the registered command with options") {
            constexpr std::string_view COMMAND_OPTIONS[] = {
                "--option-1",
                "--option-2",
                "-o",
                "-option-4=453"
            };

            // We mimic the user that types our command.
            StringType finalCommand{COMMAND_NAME};
            for (const std::string_view& option : COMMAND_OPTIONS)
                finalCommand += (StringType{" "} + StringType{option});

            inputStream.str(finalCommand);

            THEN("Its command parser should be called with the command and its options") {
                std::vector<StringType> expectedTokens{StringType{COMMAND_NAME}};
                for (const std::string_view& option : COMMAND_OPTIONS)
                    expectedTokens.push_back(StringType{option});

                EXPECT_CALL(optionParserMock, parse(expectedTokens)).Times(1);
                applicationUnderTest.run();
            }
        }
    }
}
