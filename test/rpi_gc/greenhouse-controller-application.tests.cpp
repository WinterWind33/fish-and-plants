// Copyright (c) 2022 Andrea Ballestrazzi
#include <catch2/catch_test_macros.hpp>

#include <greenhouse-controller-application.hpp>
#include <rpi_gc-config-file.hpp>

// C++ STL
#include <sstream>

namespace tests {

    void VerifyApplicationHeaderFirstLine(std::ostringstream& actual, std::string expected) {
        std::string firstLine{};
        std::istringstream linesStream{actual.str()};
        std::getline(linesStream, firstLine);

        CHECK(firstLine == expected);
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
        SECTION("It should correctly print the application header") {
            REQUIRE_NOTHROW(applicationUnderTest.run());

            tests::VerifyApplicationHeaderFirstLine(outputStream, std::string{"Greenhouse Controller "} + tests::GenerateVersionString());
        }
    }
}
