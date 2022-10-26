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
            std::getline(linesStream, lineUnderTest);
        }

        CHECK(lineUnderTest == expected);
    }

    std::string GenerateVersionString() noexcept {
        std::ostringstream stream{};
        stream << rpi_gc_VERSION_MAJOR << "." << rpi_gc_VERSION_MINOR << "." << rpi_gc_VERSION_PATCH;

        return stream.str();
    }

} // namespace tests

TEST_CASE("GreenhouseControllerApplication Unit Tests", "[unit][solitary][rpi_gc][GreenhouseControllerApplication]") {
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
    }
}
