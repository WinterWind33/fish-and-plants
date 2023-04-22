// Copyright (c) 2023 Andrea Ballestrazzi

#include <hardware-management/hardware-chip-initializer.hpp>

// Test doubles
#include <gh_log/test-doubles/logger.mock.hpp>
#include <gh_hal/test-doubles/hardware-access/board-chip-factory.mock.hpp>

// Testing core
#include <testing-core.hpp>

TEST_CASE("HardwareInitializer unit tests", "[unit][solitary][rpi_gc][hardware-management][HardwareInitializer]") {
    using rpi_gc::hardware_management::HardwareInitializer;
    using BoardChipFactoryMockImpl = testing::StrictMock<gh_hal::hardware_access::mocks::BoardChipFactoryMockImpl>;
    using BoardChipFactoryMock = gh_hal::hardware_access::mocks::BoardChipFactoryMock<BoardChipFactoryMockImpl>;

    BoardChipFactoryMock::MockImplPointer = std::make_unique<BoardChipFactoryMockImpl>();
    auto loggerMock = std::make_shared<testing::NiceMock<gh_log::mocks::LoggerMock>>();

    const HardwareInitializer<BoardChipFactoryMock> hardwareInitializerUT{std::move(loggerMock)};

    SECTION("initializeBoardChip() method should make the call to open the chip") {
        hardwareInitializerUT
    }

    SECTION("initializeBoardChip() method should make the call to open the chip only once") {

    }
}
