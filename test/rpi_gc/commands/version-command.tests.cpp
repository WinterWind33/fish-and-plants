// Copyright (c) 2023 Andrea Ballestrazzi
#include <commands/version-command.hpp>

#include <user-interface/application-strings.hpp>
#include <version/version-numbers.hpp>
#include <testing-core.hpp>

// C++ STL
#include <sstream> // For ostream test double
#include <functional> // For std::ref
#include <string>

// These tests are sociable because the version command instantiate an option object inside
// its constructor.
TEST_CASE("VersionCommand unit tests", "[unit][sociable][rpi_gc][commands][VersionCommand]") {
    using namespace rpi_gc;

    std::ostringstream fakeOutputStream{};
    VersionCommand commandUnderTest{std::ref(fakeOutputStream)};

    SECTION("execute() method") {
        SECTION("Should correctly display the name and the version when executing it") {
            const std::string expectedLine{std::string{strings::application::NAME} + " - Version " + version::getApplicationVersion().to_string()};

            [[maybe_unused]] const bool bExec{commandUnderTest.execute()};

            std::istringstream inputStream{fakeOutputStream.str()};
            std::string line{};
            std::getline(inputStream, line);

            CHECK(line == expectedLine);
        }

        SECTION("Should not block the flow after executing") {
            const bool bExec{commandUnderTest.execute()};

            CHECK(bExec);
        }
    }

    SECTION("executeAsOption() method") {
         SECTION("Should correctly display the name and the version when executing it") {
            const std::string expectedLine{std::string{strings::application::NAME} + " - Version " + version::getApplicationVersion().to_string()};

            [[maybe_unused]] const bool bExec{commandUnderTest.executeAsOption()};

            std::istringstream inputStream{fakeOutputStream.str()};
            std::string line{};
            std::getline(inputStream, line);

            CHECK(line == expectedLine);
        }

        SECTION("Should stop the flow") {
            const bool bExec{commandUnderTest.executeAsOption()};

            CHECK_FALSE(bExec);
        }
    }
}
