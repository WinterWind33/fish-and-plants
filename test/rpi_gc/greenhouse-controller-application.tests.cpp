// Copyright (c) 2022 Andrea Ballestrazzi
#include <testing-core.hpp>

#include <greenhouse-controller-application.hpp>
#include <rpi_gc-config-file.hpp>

// Test doubles
#include <gh_cmd/test-doubles/option-parser.mock.hpp>
#include <rpi_gc/test-doubles/commands/terminal-command.mock.hpp>

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

TEST_CASE("GreenhouseControllerApplication Header Lines", "[functional][rpi_gc][GreenhouseControllerApplication][application-header]") {
    using namespace rpi_gc;

    rpi_gc::OutputStringStream outputStream{};
    rpi_gc::InputStringStream inputStream{"exit"};

    GreenhouseControllerApplication applicationUnderTest{outputStream, inputStream, std::make_unique<testing::StrictMock<gh_cmd::mocks::OptionParserMock<CharType>>>()};

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

TEST_CASE("GreenhouseControllerApplication terminal input processing", "[unit][solitary][rpi_gc][GreenhouseControllerApplication][terminal-input]") {
    using namespace rpi_gc;

    GIVEN("An application controller") {
        InputStringStream inputStream{};
        OutputStringStream outputStream{};

        using TerminalOptionParserMock = testing::StrictMock<gh_cmd::mocks::OptionParserMock<CharType>>;
        std::unique_ptr<TerminalOptionParserMock> parserMockPtr{std::make_unique<TerminalOptionParserMock>()};

        TerminalOptionParserMock* terminalParserMock{parserMockPtr.get()};

        GreenhouseControllerApplication applicationUnderTest{outputStream, inputStream, std::move(parserMockPtr)};

        WHEN("A supported option (help) is given into the terminal buffer") {
            std::vector<const CharType*> strings{"rpi_gc", "--help"};

            THEN("The parser should be called one time") {
                mocks::TerminalCommandMock<CharType>* appCommandMock{};
                auto commandMockPtr = std::make_unique<testing::NiceMock<mocks::TerminalCommandMock<CharType>>>();
                appCommandMock = commandMockPtr.get();

                applicationUnderTest.setApplicationCommand(std::move(commandMockPtr));
                ON_CALL(*appCommandMock, processOptions).WillByDefault(testing::Return(true));

                testing::Expectation exp1 = EXPECT_CALL(*terminalParserMock, parse(std::vector<StringType>({StringType{"rpi_gc"}, StringType{"--help"}}))).Times(1);
                EXPECT_CALL(*terminalParserMock, getOptions).After(exp1);
                EXPECT_CALL(*terminalParserMock, getNonOptionArguments).After(exp1);
                EXPECT_CALL(*terminalParserMock, getUnknownOptions).After(exp1);

                applicationUnderTest.processInputOptions(2, strings.data());
            }
        }
    }
}
