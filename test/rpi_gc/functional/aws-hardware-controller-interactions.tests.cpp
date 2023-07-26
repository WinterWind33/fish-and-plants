// Copyright (c) 2023 Andrea Ballestrazzi
// SUTs
#include <automatic-watering/daily-cycle-automatic-watering-system.hpp>
#include <automatic-watering/hardware-controllers/daily-cycle-aws-hardware-controller.hpp>

// Dependencies
#include <automatic-watering/time-providers/configurable-daily-cycle-aws-time-provider.hpp>

// Test doubles
#include <gh_hal/test-doubles/hardware-access/board-chip.mock.hpp>
#include <gh_hal/test-doubles/hardware-access/board-digital-pin.mock.hpp>
#include <gh_log/test-doubles/logger.mock.hpp>

#include <testing-core.hpp>

// C++ STL
#include <memory>
#include <tuple>

namespace tests {
namespace constants {
// Expressed in ms
constexpr rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider::rep_type
    ACTIVATION_TIME{60};

// Expressed in ms
constexpr rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider::rep_type
    DEACTIVATION_TIME{200};

// Expressed in ms
constexpr rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider::rep_type DEACTSEP_TIME{
    5000};

constexpr gh_hal::hardware_access::BoardDigitalPin::offset_type VALVE_DUMMY_ID{26};
constexpr gh_hal::hardware_access::BoardDigitalPin::offset_type PUMP_DUMMY_ID{23};
constexpr gh_hal::hardware_access::BoardDigitalPin::offset_type NEW_VALVE_DUMMY_ID{10};
constexpr gh_hal::hardware_access::BoardDigitalPin::offset_type NEW_PUMP_DUMMY_ID{20};
} // namespace constants

using logger_mock = testing::NiceMock<gh_log::mocks::LoggerMock>;
using logger_mock_pointer = std::shared_ptr<logger_mock>;

static std::pair<logger_mock_pointer, logger_mock_pointer> createLoggersMocks() noexcept {
    return std::make_pair(std::make_shared<logger_mock>(), std::make_shared<logger_mock>());
}

using chip_mock = testing::StrictMock<gh_hal::hardware_access::mocks::BoardChipMock>;
using digital_pin_mock = testing::StrictMock<gh_hal::hardware_access::mocks::BoardDigitalPinMock>;

static std::pair<std::unique_ptr<digital_pin_mock>, digital_pin_mock*>
createDigitalPinMock() noexcept {
    std::unique_ptr<digital_pin_mock> digitalMockUniquePtr{std::make_unique<digital_pin_mock>()};
    digital_pin_mock* digitalMockPtr{digitalMockUniquePtr.get()};

    return std::make_pair(std::move(digitalMockUniquePtr), digitalMockPtr);
}

constexpr rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider::time_unit toTimeUnit(
    rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider::rep_type time) noexcept {
    return rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider::time_unit{time};
}

} // namespace tests

SCENARIO(
    "User changes the board PINs during a Daily Cycle",
    "[functional][non-regression][automatic-watering][hardware-management][user-interaction]") {
    auto [mainLoggerMock, userLoggerMock]{tests::createLoggersMocks()};
    std::mutex hardwareAccessMutex{};

    tests::chip_mock boardChipMock{};
    auto [valveMockUniquePtr, valveMockPtr]{tests::createDigitalPinMock()};
    auto [pumpMockUniquePtr, pumpMockPtr]{tests::createDigitalPinMock()};

    EXPECT_CALL(boardChipMock,
                requestDigitalPin(testing::_, tests::constants::VALVE_DUMMY_ID,
                                  gh_hal::hardware_access::DigitalPinRequestDirection::Output))
        .WillOnce(testing::Return(testing::ByMove(std::move(valveMockUniquePtr))));

    EXPECT_CALL(boardChipMock,
                requestDigitalPin(testing::_, tests::constants::PUMP_DUMMY_ID,
                                  gh_hal::hardware_access::DigitalPinRequestDirection::Output))
        .WillOnce(testing::Return(testing::ByMove(std::move(pumpMockUniquePtr))));

    rpi_gc::automatic_watering::DailyCycleAWSHardwareController awsController{
        std::ref(hardwareAccessMutex), std::ref(boardChipMock), tests::constants::VALVE_DUMMY_ID,
        tests::constants::PUMP_DUMMY_ID};
    std::atomic<rpi_gc::automatic_watering::WateringSystemHardwareController*>
        hardwareControllerAtomic{&awsController};

    rpi_gc::automatic_watering::ConfigurableDailyCycleAWSTimeProvider timeProvider{
        tests::toTimeUnit(tests::constants::ACTIVATION_TIME),
        tests::toTimeUnit(tests::constants::DEACTIVATION_TIME),
        tests::toTimeUnit(tests::constants::DEACTSEP_TIME)};
    rpi_gc::automatic_watering::DailyCycleAutomaticWateringSystem::time_provider_pointer
        awsTimeProvider{&timeProvider};

    rpi_gc::automatic_watering::DailyCycleAutomaticWateringSystem automaticWateringSystem{
        std::ref(hardwareAccessMutex), mainLoggerMock, userLoggerMock,
        std::ref(hardwareControllerAtomic), std::ref(awsTimeProvider)};

    WHEN("When the automatic watering system is started") {
        testing::Expectation valveActivationExp = EXPECT_CALL(*valveMockPtr, activate);
        testing::Expectation pumpActivationExp =
            EXPECT_CALL(*pumpMockPtr, activate).After(valveActivationExp);
        testing::Expectation valveDeactivationExp =
            EXPECT_CALL(*valveMockPtr, deactivate).After(pumpActivationExp);
        testing::Expectation pumpDeactivationExp =
            EXPECT_CALL(*pumpMockPtr, deactivate).After(valveDeactivationExp).RetiresOnSaturation();

        EXPECT_CALL(*valveMockPtr, printStatus).Times(testing::AtLeast(1));
        EXPECT_CALL(*pumpMockPtr, printStatus).Times(testing::AtLeast(1));

        automaticWateringSystem.startAutomaticWatering();

        AND_WHEN("The user changes the ID of the valve PIN") {
            testing::Expectation valveSecondDeactivationExp =
                EXPECT_CALL(*valveMockPtr, deactivate).After(pumpDeactivationExp);
            testing::Expectation releaseExp = EXPECT_CALL(boardChipMock, releaseRequest)
                                                  .Times(1)
                                                  .After(valveSecondDeactivationExp)
                                                  .WillOnce(testing::Return(true));
            EXPECT_CALL(
                boardChipMock,
                requestDigitalPin(testing::_, tests::constants::NEW_VALVE_DUMMY_ID, testing::_))
                .After(releaseExp);

            THEN("It should wait for the cycle to complete before changing the PIN") {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                REQUIRE_NOTHROW(awsController.setWaterValveDigitalOutputID(
                    tests::constants::NEW_VALVE_DUMMY_ID));
                automaticWateringSystem.requestShutdown();
            }
        }

        AND_WHEN("The user changes the ID of the pump PIN") {
            testing::Expectation pumpSecondDeactivationExp =
                EXPECT_CALL(*pumpMockPtr, deactivate).After(pumpDeactivationExp);
            testing::Expectation releaseExp = EXPECT_CALL(boardChipMock, releaseRequest)
                                                  .Times(1)
                                                  .After(pumpSecondDeactivationExp)
                                                  .WillOnce(testing::Return(true));
            EXPECT_CALL(
                boardChipMock,
                requestDigitalPin(testing::_, tests::constants::NEW_PUMP_DUMMY_ID, testing::_))
                .After(releaseExp);

            THEN("It should wait for the cycle to complete before changing the PIN") {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                REQUIRE_NOTHROW(
                    awsController.setWaterPumpDigitalOutputID(tests::constants::NEW_PUMP_DUMMY_ID));
                automaticWateringSystem.requestShutdown();
            }
        }

        AND_WHEN("The user changes both the ID of the pump PIN and the valve PIN") {
            testing::Expectation valveSecondDeactivationExp =
                EXPECT_CALL(*valveMockPtr, deactivate).After(pumpDeactivationExp);
            testing::Expectation valveReleaseExp = EXPECT_CALL(boardChipMock, releaseRequest)
                                                       .Times(1)
                                                       .After(valveSecondDeactivationExp)
                                                       .WillOnce(testing::Return(true));
            testing::Expectation newValveReqExp =
                EXPECT_CALL(
                    boardChipMock,
                    requestDigitalPin(testing::_, tests::constants::NEW_VALVE_DUMMY_ID, testing::_))
                    .After(valveReleaseExp);

            testing::Expectation pumpSecondDeactivationExp =
                EXPECT_CALL(*pumpMockPtr, deactivate).After(newValveReqExp);
            testing::Expectation pumpReleaseExp = EXPECT_CALL(boardChipMock, releaseRequest)
                                                      .Times(1)
                                                      .After(pumpSecondDeactivationExp)
                                                      .WillOnce(testing::Return(true));
            EXPECT_CALL(
                boardChipMock,
                requestDigitalPin(testing::_, tests::constants::NEW_PUMP_DUMMY_ID, testing::_))
                .After(pumpReleaseExp);

            THEN("It should wait for the cycle to complete before changing the PINs") {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                REQUIRE_NOTHROW(awsController.setWaterValveDigitalOutputID(
                    tests::constants::NEW_VALVE_DUMMY_ID));
                REQUIRE_NOTHROW(
                    awsController.setWaterPumpDigitalOutputID(tests::constants::NEW_PUMP_DUMMY_ID));
                automaticWateringSystem.requestShutdown();
            }
        }
    }
}
