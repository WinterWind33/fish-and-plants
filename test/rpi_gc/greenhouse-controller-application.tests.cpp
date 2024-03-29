// Copyright (c) 2023 Andrea Ballestrazzi
#include <testing-core.hpp>

#include <greenhouse-controller-application.hpp>
#include <version/version-numbers.hpp>

// User interface
#include <user-interface/application-strings.hpp>

// Test doubles
#include <gh_log/test-doubles/logger.mock.hpp>
#include <rpi_gc/test-doubles/abort-system/terminable-system.mock.hpp>
#include <rpi_gc/test-doubles/commands/terminal-command.mock.hpp>

// C++ STL
#include <cstdint>

namespace tests {

void VerifyLineEqual(const std::size_t lineNum, rpi_gc::OutputStringStream& actual,
                     rpi_gc::StringType expected) {
    rpi_gc::StringType lineUnderTest{};
    rpi_gc::InputStringStream linesStream{actual.str()};
    for (std::size_t i{}; i < lineNum; ++i) {
        lineUnderTest.clear();
        REQUIRE(std::getline(linesStream, lineUnderTest).good());
    }

    CHECK(lineUnderTest == expected);
}

void VerifyLineEmpty(const std::size_t lineNum, rpi_gc::OutputStringStream& actual) {
    rpi_gc::StringType lineUnderTest{};
    rpi_gc::InputStringStream linesStream{actual.str()};
    for (std::size_t i{}; i < lineNum; ++i) {
        lineUnderTest.clear();
        REQUIRE(std::getline(linesStream, lineUnderTest).good());
    }

    CHECK(lineUnderTest.empty());
}

rpi_gc::StringType GenerateVersionString() noexcept {
    rpi_gc::OutputStringStream stream{};
    stream << rpi_gc::version::getApplicationVersion();
    return stream.str();
}

} // namespace tests

TEST_CASE("GreenhouseControllerApplication Header Lines",
          "[functional][rpi_gc][GreenhouseControllerApplication][application-header]") {
    using namespace rpi_gc;
    using testing::NiceMock;

    OutputStringStream outputStream{};
    InputStringStream inputStream{"exit"};
    gc_project::ProjectController projectController{};

    GreenhouseControllerApplication applicationUnderTest{
        outputStream, inputStream, std::make_shared<NiceMock<gh_log::mocks::LoggerMock>>(),
        projectController};

    SECTION("When running the application") {
        SECTION("It should correctly print the application name and version (first line)") {
            REQUIRE_NOTHROW(applicationUnderTest.run());

            tests::VerifyLineEqual(1, outputStream,
                                   StringType{strings::application::NAME} + StringType{" "} +
                                       tests::GenerateVersionString());
        }

        SECTION("It should correctly print the copyright disclaimer (second line)") {
            REQUIRE_NOTHROW(applicationUnderTest.run());

            tests::VerifyLineEqual(2, outputStream,
                                   StringType{strings::application::COPYRIGHT_DISCLAIMER});
        }

        SECTION("It should correctly print the team credit") {
            REQUIRE_NOTHROW(applicationUnderTest.run());

            tests::VerifyLineEqual(4, outputStream,
                                   StringType{"-- "} + StringType{strings::application::TEAM_NAME} +
                                       StringType{" --"});
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

TEST_CASE("GreenhouseControllerApplication terminal input processing",
          "[unit][sociable][rpi_gc][GreenhouseControllerApplication][terminal-input]") {
    using namespace rpi_gc;

    GIVEN("An application controller") {
        using testing::NiceMock;

        InputStringStream inputStream{};
        OutputStringStream outputStream{};
        gc_project::ProjectController projectController{};
        GreenhouseControllerApplication applicationUnderTest{
            outputStream, inputStream, std::make_shared<NiceMock<gh_log::mocks::LoggerMock>>(),
            projectController};

        WHEN("A supported option (help) is given into the terminal buffer") {
            std::vector<std::string> strings{std::string{strings::application::EXECUTABLE_NAME},
                                             std::string{"--help"}};

            THEN("The application command should correctly parse the input line") {
                auto commandMockPtr =
                    std::make_unique<testing::StrictMock<mocks::TerminalCommandMock<CharType>>>();
                testing::Expectation exp1 =
                    EXPECT_CALL(*commandMockPtr,
                                processInputOptions(std::vector<StringType>{
                                    strings::application::EXECUTABLE_NAME.data(), "--help"}))
                        .Times(1)
                        .WillOnce(testing::Return(true));

                applicationUnderTest.setApplicationCommand(std::move(commandMockPtr));
                applicationUnderTest.processInputOptions(strings);
            }
        }
    }
}

TEST_CASE("GreenhouseControllerApplication termination unit tests",
          "[unit][sociable][rpi_gc][GreenhouseControllerApplication][termination]") {
    using namespace rpi_gc;

    GIVEN("An application controller") {
        using testing::NiceMock;

        InputStringStream inputStream{};
        OutputStringStream outputStream{};
        gc_project::ProjectController projectController{};
        GreenhouseControllerApplication applicationUnderTest{
            outputStream, inputStream, std::make_shared<NiceMock<gh_log::mocks::LoggerMock>>(),
            projectController};

        AND_GIVEN("A registered terminable system") {
            using testing::StrictMock;
            using TerminableSystemMock = StrictMock<abort_system::mocks::TerminableSystemMock>;

            std::shared_ptr<TerminableSystemMock> terminableSystemMock{
                std::make_shared<TerminableSystemMock>()};
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
