// Copyright (c) 2022 Andrea Ballestrazzi
#include <catch2/catch_test_macros.hpp>

#include <greenhouse-controller-application.hpp>
#include <rpi_gc-config-file.hpp>

// C++ STL
#include <sstream>
#include <cstdint>

namespace tests {

    void VerifyApplicationHeaderLine(const std::size_t lineNum, std::ostringstream& actual, std::string expected) {
        std::string lineUnderTest{};
        std::istringstream linesStream{actual.str()};
        for(std::size_t i{}; i < lineNum; ++i) {
            lineUnderTest.clear();
            REQUIRE(std::getline(linesStream, lineUnderTest).good());
        }

        CHECK(lineUnderTest == expected);
    }

    void VerifyEmptyLine(const std::size_t lineNum, std::ostringstream& actual) {
        std::string lineUnderTest{};
        std::istringstream linesStream{actual.str()};
        for(std::size_t i{}; i < lineNum; ++i) {
            lineUnderTest.clear();
            REQUIRE(std::getline(linesStream, lineUnderTest).good());
        }

        CHECK(lineUnderTest.empty());
    }

    std::string GenerateVersionString() noexcept {
        std::ostringstream stream{};
        stream << rpi_gc_VERSION_MAJOR << "." << rpi_gc_VERSION_MINOR << "." << rpi_gc_VERSION_PATCH;

        return stream.str();
    }

} // namespace tests

TEST_CASE("Application Header Lines", "[unit][solitary][rpi_gc][GreenhouseControllerApplication][application-header]") {
    using namespace rpi_gc;

    std::ostringstream outputStream{};
    std::istringstream inputStream{};

    GreenhouseControllerApplication applicationUnderTest{outputStream, inputStream};

    SECTION("When running the application") {
        SECTION("It should correctly print the application name and version (first line)") {
            REQUIRE_NOTHROW(applicationUnderTest.run());

            tests::VerifyApplicationHeaderLine(1, outputStream, std::string{"Greenhouse Controller "} + tests::GenerateVersionString());
        }

        SECTION("It should correctly print the copyright disclaimer (second line)") {
            REQUIRE_NOTHROW(applicationUnderTest.run());

            tests::VerifyApplicationHeaderLine(2, outputStream, "Copyright (c) 2022 Andrea Ballestrazzi");
        }

        SECTION("It should correctly print the team credit") {
            REQUIRE_NOTHROW(applicationUnderTest.run());

            tests::VerifyApplicationHeaderLine(4, outputStream, "-- Fish&Plants Team --");
        }

        SECTION("It should add an end line between the disclaimer and the team credit") {
            REQUIRE_NOTHROW(applicationUnderTest.run());

            tests::VerifyEmptyLine(3, outputStream);
        }

        SECTION("It should add an end line after the team credit") {
            REQUIRE_NOTHROW(applicationUnderTest.run());

            tests::VerifyEmptyLine(5, outputStream);
        }
    }
}
