// Copyright (c) 2023 Andrea Ballestrazzi
#include <commands/project-command.hpp>
#include <user-interface/commands-strings.hpp>

#include <testing-core.hpp>

TEST_CASE("ProjectCommand unit tests", "[unit][solitary][rpi_gc][commands][ProjectCommand]") {
    using namespace rpi_gc;

    SECTION("getName() should return the correct name") {
        const commands::ProjectCommand commandUnderTest{};

        CHECK(commandUnderTest.getName() == strings::commands::PROJECT);
    }
}
