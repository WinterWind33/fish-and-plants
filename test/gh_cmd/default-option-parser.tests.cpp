// Copyright (c) 2022 Andrea Ballestrazzi
#include <testing-core.hpp>

#include <gh_cmd/gh_cmd.hpp>

// C++ STL
#include <string_view>

TEST_CASE("DefaultOptionParser parse() method unit tests", "[unit][sociable][gh_cmd][DefaultOptionParser]") {
    using namespace gh_cmd;

    GIVEN("A DefaultOptionParser") {
        DefaultOptionParser<char> parserUnderTest{};

        WHEN("A single switch is added") {
            constexpr char SWITCH_SHORT_NAME{'h'};
            constexpr std::string_view SWITCH_LONG_NAME{"help"};
            constexpr std::string_view SWITCH_DESCRIPTION{"Prints the help page"};
            std::shared_ptr<Switch<char>> switchOption{std::make_shared<Switch<char>>(SWITCH_SHORT_NAME, std::string{SWITCH_LONG_NAME}, std::string{SWITCH_DESCRIPTION})};

            parserUnderTest.addSwitch(switchOption);

            AND_WHEN("parse() is invoked with a command with the switch - short name") {
                parserUnderTest.parse({"dummy-command", "-h"});

                THEN("It shouldn\'t have any unknown options") {
                    CHECK(parserUnderTest.getUnknownOptions().empty());
                }

                THEN("It shouldn\'t have any non-option arguments") {
                    CHECK(parserUnderTest.getNonOptionArguments().empty());
                }

                THEN("The switch option should be set") {
                    CHECK(switchOption->isSet());
                }
            }

            AND_WHEN("parse() is invoked with a command with the switch - long name") {
                parserUnderTest.parse({"dummy-command", "--help"});

                THEN("It shouldn\'t have any unknown options") {
                    CHECK(parserUnderTest.getUnknownOptions().empty());
                }

                THEN("It shouldn\'t have any non-option arguments") {
                    CHECK(parserUnderTest.getNonOptionArguments().empty());
                }

                THEN("The switch option should be set") {
                    CHECK(switchOption->isSet());
                }
            }

            AND_WHEN("parse() is invoked with unknown arguments") {
                constexpr std::string_view UNKNOWN_OPTION{"-ukwn"};

                parserUnderTest.parse({"dummy-command", std::string{UNKNOWN_OPTION}});

                THEN("It should have the correct unknown options count") {
                    CHECK(parserUnderTest.getUnknownOptions().size() == 1);
                }

                THEN("It should have the correct unknown options") {
                    REQUIRE_FALSE(parserUnderTest.getUnknownOptions().empty());

                    CHECK(parserUnderTest.getUnknownOptions()[0] == UNKNOWN_OPTION);
                }

                THEN("It shouldn\'t have any non-option arguments") {
                    CHECK(parserUnderTest.getNonOptionArguments().empty());
                }

                THEN("The switch option should not be set") {
                    CHECK_FALSE(switchOption->isSet());
                }
            }

            AND_WHEN("parse() is invoked with the switch (long name) and non-option arguments") {
                constexpr std::string_view NON_OPTION_ARG{"test-arg"};

                parserUnderTest.parse({"dummy-command", "--help", "--", std::string{NON_OPTION_ARG}});

                THEN("It shouldn\'t have any unknown options") {
                    CHECK(parserUnderTest.getUnknownOptions().empty());
                }

                THEN("It should have the correct non-option args count") {
                    CHECK(parserUnderTest.getNonOptionArguments().size() == 1);
                }

                THEN("It should have the correct non-option args") {
                    REQUIRE_FALSE(parserUnderTest.getNonOptionArguments().empty());

                    CHECK(parserUnderTest.getNonOptionArguments()[0] == NON_OPTION_ARG);
                }

                THEN("The switch option should be set") {
                    CHECK(switchOption->isSet());
                }
            }
        }
    }
}
