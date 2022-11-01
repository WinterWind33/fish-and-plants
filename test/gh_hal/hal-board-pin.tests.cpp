// Copyright (c) 2022 Andrea Ballestrazzi
#include <testing-core.hpp>

#include <gh_hal/hal-board-pin.hpp>

// C++ STL
#include <type_traits>

TEST_CASE("HALBoardPin Static Tests", "[static][gh_hal][HALBoardPin]") {
    using namespace gh_hal;

    SECTION("Native board pin type") {
#ifdef USE_CPPGPIO
        STATIC_REQUIRE(std::is_same_v<HALBoardPin::native_type, GPIO::DigitalOut>);
#else
        STATIC_REQUIRE(std::is_same_v<HALBoardPin::native_type, details::FakeBoardPinNativeType>);
#endif
    }
}
