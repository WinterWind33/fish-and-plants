// Copyright (c) 2023 Andrea Ballestrazzi

#include <hardware-management/hardware-chip-initializer.hpp>

// Test doubles
#include <gh_hal/test-doubles/hardware-access/board-chip-factory.mock.hpp>
#include <gh_log/test-doubles/logger.mock.hpp>

// Testing core
#include <testing-core.hpp>

// C++ STL
#include <string_view>

namespace tests::constants {
constexpr std::string_view RASPBERRY_PI_3B_PLUS_GPIO0CHIP_PATH{"/dev/gpiochip0"};
} // namespace tests::constants

TEST_CASE("HardwareInitializer unit tests",
          "[unit][solitary][rpi_gc][hardware-management][HardwareInitializer]") {
    using rpi_gc::hardware_management::HardwareInitializer;
    using BoardChipFactoryMockImpl =
        testing::StrictMock<gh_hal::hardware_access::mocks::BoardChipFactoryMockImpl>;
    using BoardChipFactoryMock =
        gh_hal::hardware_access::mocks::BoardChipFactoryMock<BoardChipFactoryMockImpl>;

    BoardChipFactoryMock::MockImplPointer = std::make_unique<BoardChipFactoryMockImpl>();
    auto loggerMock = std::make_shared<testing::NiceMock<gh_log::mocks::LoggerMock>>();

    const HardwareInitializer<BoardChipFactoryMock> hardwareInitializerUT{std::move(loggerMock)};

    SECTION("initializeBoardChip() method should make the call to open the chip only once") {
        const std::filesystem::path chipPath{tests::constants::RASPBERRY_PI_3B_PLUS_GPIO0CHIP_PATH};

        EXPECT_CALL(*BoardChipFactoryMock::MockImplPointer, openChipByPathMock(chipPath)).Times(1);

        hardwareInitializerUT.initializeBoardChip(chipPath);

        // We need to clear the MockImplPointer because GMock must complete the
        // expectations during the mock destruction. Being this a static variable,
        // we need to do this manually.
        BoardChipFactoryMock::MockImplPointer.reset();
    }
}
