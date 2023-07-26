// Copyright (c) 2023 Andrea Ballestrazzi

#include <gh_hal/hardware-access/board-chip.hpp>

// Test frameworks.
#include <testing-core.hpp>

TEST_CASE("BoardChipFactory static tests",
          "[static][gh_hal][hardware-access][board-chip][BoardChipFactory]") {
    using gh_hal::hardware_access::BoardChipFactory;

    STATIC_CHECK(gh_hal::hardware_access::BoardChipFactoryType<BoardChipFactory>);
}
