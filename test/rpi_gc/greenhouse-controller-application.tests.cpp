// Copyright (c) 2022 Andrea Ballestrazzi
#include <testing-core.hpp>

#include <greenhouse-controller-application.hpp>
#include <rpi_gc-config-file.hpp>

// User interface
#include <user-interface/application-strings.hpp>

// Test doubles
#include <rpi_gc/test-doubles/commands/terminal-command.mock.hpp>
#include <rpi_gc/test-doubles/abort-system/terminable-system.mock.hpp>

// C++ STL
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

    OutputStringStream outputStream{};
    InputStringStream inputStream{"exit"};

    GreenhouseControllerApplication applicationUnderTest{outputStream, inputStream};

    SECTION("When running the application") {
        SECTION("It should correctly print the application name and version (first line)") {
            REQUIRE_NOTHROW(applicationUnderTest.run());

            tests::VerifyLineEqual(1, outputStream, StringType{strings::application::NAME} + StringType{" "} + tests::GenerateVersionString());
        }

        SECTION("It should correctly print the copyright disclaimer (second line)") {
            REQUIRE_NOTHROW(applicationUnderTest.run());

            tests::VerifyLineEqual(2, outputStream, StringType{strings::application::COPYRIGHT_DISCLAIMER});
        }

        SECTION("It should correctly print the team credit") {
            REQUIRE_NOTHROW(applicationUnderTest.run());

            tests::VerifyLineEqual(4, outputStream, StringType{"-- "} + StringType{strings::application::TEAM_NAME} + StringType{" --"});
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
        GreenhouseControllerApplication applicationUnderTest{outputStream, inputStream};

        WHEN("A supported option (help) is given into the terminal buffer") {
            std::vector<const CharType*> strings{strings::application::EXECUTABLE_NAME.data(), "--help"};

            THEN("The application command should correctly parse the input line") {
                auto commandMockPtr = std::make_unique<testing::StrictMock<mocks::TerminalCommandMock<CharType>>>();
                testing::Expectation exp1 = EXPECT_CALL(*commandMockPtr, processInputOptions(std::vector<StringType>{strings::application::EXECUTABLE_NAME.data(), "--help"}))
                   .Times(1)
                   .WillOnce(testing::Return(true));

                applicationUnderTest.setApplicationCommand(std::move(commandMockPtr));
                applicationUnderTest.processInputOptions(2, strings.data());
            }
        }
    }
}

TEST_CASE("GreenhouseControllerApplication termination unit tests", "[unit][solitary][rpi_gc][GreenhouseControllerApplication][termination]") {
    using namespace rpi_gc;

    GIVEN("An application controller") {
        InputStringStream inputStream{};
        OutputStringStream outputStream{};
        GreenhouseControllerApplication applicationUnderTest{outputStream, inputStream};

        AND_GIVEN("A registered terminable system") {
            using testing::StrictMock;
            using TerminableSystemMock = StrictMock<abort_system::mocks::TerminableSystemMock>;

            std::shared_ptr<TerminableSystemMock> terminableSystemMock{std::make_shared<TerminableSystemMock>()};
            applicationUnderTest.addTerminableSystem(terminableSystemMock);

            WHEN("The application is run") {
                THEN("The terminable system should be correctly called only one time") {
                    EXPECT_CALL(*terminableSystemMock, requestShutdown).Times(1);

                    applicationUnderTest.run();
                }
            }
        }
    }
}
