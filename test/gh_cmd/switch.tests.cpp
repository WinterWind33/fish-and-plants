// Copyright (c) 2022 Andrea Ballestrazzi
#include <catch2/catch_test_macros.hpp>
#include <gh_cmd/gh_cmd.hpp>

// Test doubles
#include <gh_cmd/test-doubles/option-visitor.mock.hpp>

// C++ STL
#include <string_view>

TEST_CASE("Switch sociable unit tests", "[unit][sociable][gh_cmd][Switch]") {
    using namespace gh_cmd;

    SECTION("Getters") {
        constexpr char EXPECTED_SHORT_NAME{'h'};
        constexpr std::string_view EXPECTED_LONG_NAME{"help"};
        constexpr std::string_view EXPECTED_DESCRIPTION{"Produces help message"};

        Switch<char> switchUnderTest{EXPECTED_SHORT_NAME, std::string{EXPECTED_LONG_NAME}, std::string{EXPECTED_DESCRIPTION}};

        SECTION("It should retrieve the correct short name") {
            CHECK(switchUnderTest.getShortName() == EXPECTED_SHORT_NAME);
        }

        SECTION("It should retrieve the correct long name") {
            CHECK(switchUnderTest.getLongName() == EXPECTED_LONG_NAME);
        }

        SECTION("It should retrieve the correct description") {
            CHECK(switchUnderTest.getDescription() == EXPECTED_DESCRIPTION);
        }
    }
}
