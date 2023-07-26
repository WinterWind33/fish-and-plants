// Copyright (c) 2023 Andrea Ballestrazzi
#include <automatic-watering/hardware-controllers/daily-cycle-aws-hardware-controller.hpp>

// Test doubles
#include <gh_hal/test-doubles/hardware-access/board-chip.mock.hpp>
#include <gh_hal/test-doubles/hardware-access/board-digital-pin.mock.hpp>

#include <testing-core.hpp>

namespace tests::constants {
constexpr rpi_gc::automatic_watering::DailyCycleAWSHardwareController::digital_output_id
    VALVE_DUMMY_ID{26};
constexpr rpi_gc::automatic_watering::DailyCycleAWSHardwareController::digital_output_id
    PUMP_DUMMY_ID{23};
constexpr rpi_gc::automatic_watering::DailyCycleAWSHardwareController::digital_output_id
    NEW_VALVE_DUMMY_ID{10};
constexpr rpi_gc::automatic_watering::DailyCycleAWSHardwareController::digital_output_id
    NEW_PUMP_DUMMY_ID{20};
} // namespace tests::constants

TEST_CASE("DailyCycleAWSHardwareController unit tests",
          "[unit][solitary][rpi_gc][automatic-watering][hardware-controllers][daily-cycle-aws-"
          "hardware-controller.tests.cpp]") {
    using namespace rpi_gc::automatic_watering;
    using chip_mock = gh_hal::hardware_access::mocks::BoardChipMock;
    using digital_pin_mock =
        testing::StrictMock<gh_hal::hardware_access::mocks::BoardDigitalPinMock>;

    // TO instance the digital outputs mocks we need to make three steps
    // because we need to have the pointers to the mocks (not the interface!!) for later to set
    // expectations.
    std::unique_ptr<digital_pin_mock> valveMockUniquePtr{std::make_unique<digital_pin_mock>()},
        pumpMockUniquePtr{std::make_unique<digital_pin_mock>()};
    digital_pin_mock* const valveDigitalOutPtr{valveMockUniquePtr.get()};
    digital_pin_mock* const pumpDigitalOutPtr{pumpMockUniquePtr.get()};
    std::unique_ptr<DailyCycleAWSHardwareController::digital_output_type> valveDigitalOutMock{
        std::move(valveMockUniquePtr)},
        pumpDigitalOutMock{std::move(pumpMockUniquePtr)};
    std::mutex hardwareAccessMutex{};

    std::unique_ptr<DailyCycleAWSHardwareController> hardwareControllerUnderTest{};

    WHEN("Creating a new DC-AWS hardware controller") {
        THEN("It should request the digital pins of the valve and the pump") {
            testing::StrictMock<chip_mock> boardChipMock{};

            EXPECT_CALL(
                boardChipMock,
                requestDigitalPin(testing::_, tests::constants::VALVE_DUMMY_ID,
                                  gh_hal::hardware_access::DigitalPinRequestDirection::Output))
                .Times(1)
                .WillOnce(testing::Return(nullptr));

            EXPECT_CALL(
                boardChipMock,
                requestDigitalPin(testing::_, tests::constants::PUMP_DUMMY_ID,
                                  gh_hal::hardware_access::DigitalPinRequestDirection::Output))
                .Times(1)
                .WillOnce(testing::Return(nullptr));

            hardwareControllerUnderTest = std::make_unique<DailyCycleAWSHardwareController>(
                std::ref(hardwareAccessMutex), std::ref(boardChipMock),
                tests::constants::VALVE_DUMMY_ID, tests::constants::PUMP_DUMMY_ID);
        }

        THEN("The object should have the correct pins") {
            testing::NiceMock<chip_mock> boardChipMock{};
            ON_CALL(boardChipMock,
                    requestDigitalPin(testing::_, tests::constants::VALVE_DUMMY_ID,
                                      gh_hal::hardware_access::DigitalPinRequestDirection::Output))
                .WillByDefault(testing::Return(testing::ByMove(std::move(valveDigitalOutMock))));

            ON_CALL(boardChipMock,
                    requestDigitalPin(testing::_, tests::constants::PUMP_DUMMY_ID,
                                      gh_hal::hardware_access::DigitalPinRequestDirection::Output))
                .WillByDefault(testing::Return(testing::ByMove(std::move(pumpDigitalOutMock))));

            hardwareControllerUnderTest = std::make_unique<DailyCycleAWSHardwareController>(
                std::ref(hardwareAccessMutex), std::ref(boardChipMock),
                tests::constants::VALVE_DUMMY_ID, tests::constants::PUMP_DUMMY_ID);

            CHECK(hardwareControllerUnderTest->getWaterValveDigitalOut() == valveDigitalOutPtr);
            CHECK(hardwareControllerUnderTest->getWaterPumpDigitalOut() == pumpDigitalOutPtr);
        }
    }

    GIVEN("A DailyCycleAWSHardwareController object") {
        testing::StrictMock<chip_mock> boardChipMock{};

        EXPECT_CALL(boardChipMock,
                    requestDigitalPin(testing::_, tests::constants::VALVE_DUMMY_ID,
                                      gh_hal::hardware_access::DigitalPinRequestDirection::Output))
            .Times(1)
            .WillOnce(testing::Return(testing::ByMove(std::move(valveDigitalOutMock))));

        EXPECT_CALL(boardChipMock,
                    requestDigitalPin(testing::_, tests::constants::PUMP_DUMMY_ID,
                                      gh_hal::hardware_access::DigitalPinRequestDirection::Output))
            .Times(1)
            .WillOnce(testing::Return(testing::ByMove(std::move(pumpDigitalOutMock))));
        ;

        hardwareControllerUnderTest = std::make_unique<DailyCycleAWSHardwareController>(
            std::ref(hardwareAccessMutex), std::ref(boardChipMock),
            tests::constants::VALVE_DUMMY_ID, tests::constants::PUMP_DUMMY_ID);

        WHEN("A new valve digital out is requested") {
            THEN("A new digital pin must be requested deactivating the old one") {
                EXPECT_CALL(*valveDigitalOutPtr, deactivate).Times(1);

                const std::vector<gh_hal::hardware_access::BoardDigitalPin::offset_type> offsetsVec{
                    tests::constants::VALVE_DUMMY_ID};
                EXPECT_CALL(boardChipMock, releaseRequest(offsetsVec))
                    .Times(1)
                    .WillOnce(testing::Return(true));

                EXPECT_CALL(
                    boardChipMock,
                    requestDigitalPin(testing::_, tests::constants::NEW_VALVE_DUMMY_ID,
                                      gh_hal::hardware_access::DigitalPinRequestDirection::Output))
                    .Times(1)
                    .WillOnce(testing::Return(nullptr));

                REQUIRE_NOTHROW(hardwareControllerUnderTest->setWaterValveDigitalOutputID(
                    tests::constants::NEW_VALVE_DUMMY_ID));
            }

            THEN("The sequence of creation of the new pun must be correct") {
                testing::Expectation valveShutoffExp =
                    EXPECT_CALL(*valveDigitalOutPtr, deactivate).Times(1);

                const std::vector<gh_hal::hardware_access::BoardDigitalPin::offset_type> offsetsVec{
                    tests::constants::VALVE_DUMMY_ID};
                testing::Expectation releaseReqExp =
                    EXPECT_CALL(boardChipMock, releaseRequest(offsetsVec))
                        .Times(1)
                        .After(valveShutoffExp)
                        .WillOnce(testing::Return(true));
                ;

                EXPECT_CALL(
                    boardChipMock,
                    requestDigitalPin(testing::_, tests::constants::NEW_VALVE_DUMMY_ID,
                                      gh_hal::hardware_access::DigitalPinRequestDirection::Output))
                    .Times(1)
                    .After(releaseReqExp)
                    .WillOnce(testing::Return(nullptr));

                REQUIRE_NOTHROW(hardwareControllerUnderTest->setWaterValveDigitalOutputID(
                    tests::constants::NEW_VALVE_DUMMY_ID));
            }

            THEN("The state of the hardware controller must be correctly updated") {
                EXPECT_CALL(*valveDigitalOutPtr, deactivate).Times(1);

                const std::vector<gh_hal::hardware_access::BoardDigitalPin::offset_type> offsetsVec{
                    tests::constants::VALVE_DUMMY_ID};
                EXPECT_CALL(boardChipMock, releaseRequest(offsetsVec))
                    .Times(1)
                    .WillOnce(testing::Return(true));
                ;

                std::unique_ptr<digital_pin_mock> newValveDigitalOutUniquePtr{
                    std::make_unique<digital_pin_mock>()};
                digital_pin_mock* newValveDigitalOutPtr{newValveDigitalOutUniquePtr.get()};
                std::unique_ptr<DailyCycleAWSHardwareController::digital_output_type>
                    newValveDigitalOut{std::move(newValveDigitalOutUniquePtr)};

                EXPECT_CALL(
                    boardChipMock,
                    requestDigitalPin(testing::_, tests::constants::NEW_VALVE_DUMMY_ID,
                                      gh_hal::hardware_access::DigitalPinRequestDirection::Output))
                    .Times(1)
                    .WillOnce(testing::Return(testing::ByMove(std::move(newValveDigitalOut))));

                REQUIRE_NOTHROW(hardwareControllerUnderTest->setWaterValveDigitalOutputID(
                    tests::constants::NEW_VALVE_DUMMY_ID));

                CHECK(hardwareControllerUnderTest->getWaterValveDigitalOut() ==
                      newValveDigitalOutPtr);
            }
        }

        WHEN("A new pump digital out is requested") {
            THEN("A new digital pin must be requested deactivating the old one") {
                EXPECT_CALL(*pumpDigitalOutPtr, deactivate).Times(1);

                const std::vector<gh_hal::hardware_access::BoardDigitalPin::offset_type> offsetsVec{
                    tests::constants::PUMP_DUMMY_ID};
                EXPECT_CALL(boardChipMock, releaseRequest(offsetsVec))
                    .Times(1)
                    .WillOnce(testing::Return(true));

                EXPECT_CALL(
                    boardChipMock,
                    requestDigitalPin(testing::_, tests::constants::NEW_PUMP_DUMMY_ID,
                                      gh_hal::hardware_access::DigitalPinRequestDirection::Output))
                    .Times(1)
                    .WillOnce(testing::Return(nullptr));

                REQUIRE_NOTHROW(hardwareControllerUnderTest->setWaterPumpDigitalOutputID(
                    tests::constants::NEW_PUMP_DUMMY_ID));
            }

            THEN("The sequence of creation of the new pun must be correct") {
                testing::Expectation pumpShutoffExp =
                    EXPECT_CALL(*pumpDigitalOutPtr, deactivate).Times(1);

                const std::vector<gh_hal::hardware_access::BoardDigitalPin::offset_type> offsetsVec{
                    tests::constants::PUMP_DUMMY_ID};
                testing::Expectation releaseReqExp =
                    EXPECT_CALL(boardChipMock, releaseRequest(offsetsVec))
                        .Times(1)
                        .After(pumpShutoffExp)
                        .WillOnce(testing::Return(true));
                ;

                EXPECT_CALL(
                    boardChipMock,
                    requestDigitalPin(testing::_, tests::constants::NEW_PUMP_DUMMY_ID,
                                      gh_hal::hardware_access::DigitalPinRequestDirection::Output))
                    .Times(1)
                    .After(releaseReqExp)
                    .WillOnce(testing::Return(nullptr));

                REQUIRE_NOTHROW(hardwareControllerUnderTest->setWaterPumpDigitalOutputID(
                    tests::constants::NEW_PUMP_DUMMY_ID));
            }

            THEN("The state of the hardware controller must be correctly updated") {
                EXPECT_CALL(*pumpDigitalOutPtr, deactivate).Times(1);

                const std::vector<gh_hal::hardware_access::BoardDigitalPin::offset_type> offsetsVec{
                    tests::constants::PUMP_DUMMY_ID};
                EXPECT_CALL(boardChipMock, releaseRequest(offsetsVec))
                    .Times(1)
                    .WillOnce(testing::Return(true));
                ;

                std::unique_ptr<digital_pin_mock> newPumpDigitalOutUniquePtr{
                    std::make_unique<digital_pin_mock>()};
                digital_pin_mock* newPumpDigitalOutPtr{newPumpDigitalOutUniquePtr.get()};
                std::unique_ptr<DailyCycleAWSHardwareController::digital_output_type>
                    newPumpDigitalOut{std::move(newPumpDigitalOutUniquePtr)};

                EXPECT_CALL(
                    boardChipMock,
                    requestDigitalPin(testing::_, tests::constants::NEW_PUMP_DUMMY_ID,
                                      gh_hal::hardware_access::DigitalPinRequestDirection::Output))
                    .Times(1)
                    .WillOnce(testing::Return(testing::ByMove(std::move(newPumpDigitalOut))));

                REQUIRE_NOTHROW(hardwareControllerUnderTest->setWaterPumpDigitalOutputID(
                    tests::constants::NEW_PUMP_DUMMY_ID));

                CHECK(hardwareControllerUnderTest->getWaterPumpDigitalOut() ==
                      newPumpDigitalOutPtr);
            }
        }
    }
}
