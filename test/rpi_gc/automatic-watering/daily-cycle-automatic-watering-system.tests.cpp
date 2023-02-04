// Copyright (c) 2023 Andrea Ballestrazzi

#include <automatic-watering/daily-cycle-automatic-watering-system.hpp>
#include <automatic-watering/time-providers/daily-cycle-aws-time-provider.hpp>

// Test Doubles
#include <rpi_gc/test-doubles/automatic-watering/time-providers/watering-system-time-provider.mock.hpp>
#include <rpi_gc/test-doubles/automatic-watering/hardware-controllers/watering-system-hardware-controller.mock.hpp>
#include <gh_log/test-doubles/logger.mock.hpp>
#include <gh_hal/test-doubles/hal-digital-output.mock.hpp>

#include <testing-core.hpp>

// C++ STL
#include <thread>
#include <chrono>
#include <atomic>

namespace tests {
    constexpr std::chrono::milliseconds WAIT_FOR_THREAD_TO_START{100};
} // namespace testing

TEST_CASE("DailyCycleAutomaticWateringSystem unit tests", "[unit][solitary][rpi_gc][automatic-watering][DailyCycleAutomaticWateringSystem]") {
    using namespace rpi_gc::automatic_watering;
    using testing::NiceMock;
    using testing::StrictMock;

    std::shared_ptr<NiceMock<gh_log::mocks::LoggerMock>> mainLoggerMock{std::make_shared<NiceMock<gh_log::mocks::LoggerMock>>()};
    std::shared_ptr<NiceMock<gh_log::mocks::LoggerMock>> userLoggerMock{std::make_shared<NiceMock<gh_log::mocks::LoggerMock>>()};

    std::unique_ptr<StrictMock<mocks::WateringSystemHardwareControllerMock>> hardwareControllerMock{
        std::make_unique<StrictMock<mocks::WateringSystemHardwareControllerMock>>()
    };
    mocks::WateringSystemHardwareControllerMock& hardwareControllerMockRef{*hardwareControllerMock};
    std::atomic<WateringSystemHardwareController*> atomicHardwareController{hardwareControllerMock.get()};

    std::shared_ptr<StrictMock<mocks::WateringSystemTimeProviderMock>> timeProviderMock{
        std::make_shared<StrictMock<mocks::WateringSystemTimeProviderMock>>()
    };

    std::atomic<WateringSystemTimeProvider*> timeProviderAtomic{timeProviderMock.get()};

    DailyCycleAutomaticWateringSystem awsUnderTest{
        mainLoggerMock,
        userLoggerMock,
        std::ref(atomicHardwareController),
        std::ref(timeProviderAtomic)
    };

    GIVEN("A time configuration for the watering system") {
        constexpr WateringSystemTimeProvider::time_unit ACTIVATION_TIME{60};
        constexpr WateringSystemTimeProvider::time_unit DEACTIVATION_TIME{100};
        constexpr WateringSystemTimeProvider::time_unit VALVE_PUMP_SEPARATION_TIME{100};

        EXPECT_CALL(*timeProviderMock, getWateringSystemActivationDuration)
            .WillRepeatedly(testing::Return(ACTIVATION_TIME));
        EXPECT_CALL(*timeProviderMock, getWateringSystemDeactivationDuration)
            .WillRepeatedly(testing::Return(DEACTIVATION_TIME));
        EXPECT_CALL(*timeProviderMock, getPumpValveDeactivationTimeSeparation)
            .WillRepeatedly(testing::Return(VALVE_PUMP_SEPARATION_TIME));

        NiceMock<gh_hal::mocks::HALDigitalOutputMock> waterValveOutput{}, waterPumpOutput{};
        EXPECT_CALL(hardwareControllerMockRef, getWaterValveDigitalOut).WillRepeatedly(testing::Return(&waterValveOutput));
        EXPECT_CALL(hardwareControllerMockRef, getWaterPumpDigitalOut).WillRepeatedly(testing::Return(&waterPumpOutput));

        WHEN("The watering system is activated") {
            using testing::Expectation;

            THEN("It should correctly activate the water valve before the water pump") {
                Expectation waterValveOnExp = EXPECT_CALL(waterValveOutput, turnOn)
                    .Times(1);
                Expectation waterPumpOnExp = EXPECT_CALL(waterPumpOutput, turnOn)
                    .Times(1)
                    .After(waterValveOnExp);

                EXPECT_CALL(waterValveOutput, turnOff);
                EXPECT_CALL(waterPumpOutput, turnOff);

                awsUnderTest.startAutomaticWatering();

                // We must wait the start of the thread before requesting a stop.
                std::this_thread::sleep_for(tests::WAIT_FOR_THREAD_TO_START);

                awsUnderTest.requestShutdown();
            }

            THEN("It should correctly deactivate the water valve before the water pump") {
                EXPECT_CALL(waterValveOutput, turnOn);
                EXPECT_CALL(waterPumpOutput, turnOn);

                Expectation exp1 = EXPECT_CALL(waterValveOutput, turnOff)
                    .Times(1);

                EXPECT_CALL(waterPumpOutput, turnOff)
                    .Times(1)
                    .After(exp1);

                awsUnderTest.startAutomaticWatering();

                // We must wait the start of the thread before requesting a stop.
                std::this_thread::sleep_for(tests::WAIT_FOR_THREAD_TO_START);

                awsUnderTest.requestShutdown();
            }
        }
    }
}

TEST_CASE("DailyCycleAutomaticWateringSystem integration tests", "[integration][rpi_gc][automatic-watering][DailyCycleAutomaticWateringSystem]") {
    using testing::NiceMock;
    using testing::StrictMock;
    using namespace rpi_gc::automatic_watering;

    std::shared_ptr<NiceMock<gh_log::mocks::LoggerMock>> mainLoggerMock{std::make_shared<NiceMock<gh_log::mocks::LoggerMock>>()};
    std::shared_ptr<NiceMock<gh_log::mocks::LoggerMock>> userLoggerMock{std::make_shared<NiceMock<gh_log::mocks::LoggerMock>>()};

    std::unique_ptr<StrictMock<mocks::WateringSystemHardwareControllerMock>> hardwareControllerMock{
        std::make_unique<StrictMock<mocks::WateringSystemHardwareControllerMock>>()
    };
    mocks::WateringSystemHardwareControllerMock& hardwareControllerMockRef{*hardwareControllerMock};
    std::atomic<WateringSystemHardwareController*> atomicHardwareController{hardwareControllerMock.get()};

    NiceMock<gh_hal::mocks::HALDigitalOutputMock> waterValveOutput{}, waterPumpOutput{};
    EXPECT_CALL(hardwareControllerMockRef, getWaterValveDigitalOut).WillRepeatedly(testing::Return(&waterValveOutput));
    EXPECT_CALL(hardwareControllerMockRef, getWaterPumpDigitalOut).WillRepeatedly(testing::Return(&waterPumpOutput));

    std::shared_ptr<DailyCycleAWSTimeProvider> awsTimeProvider{std::make_shared<DailyCycleAWSTimeProvider>()};
    std::atomic<WateringSystemTimeProvider*> timeProviderAtomic{awsTimeProvider.get()};

    DailyCycleAutomaticWateringSystem awsUnderTest{
        mainLoggerMock,
        userLoggerMock,
        std::ref(atomicHardwareController),
        std::ref(timeProviderAtomic)
    };

    WHEN("The automatic watering system is activated") {
        awsUnderTest.startAutomaticWatering();

        AND_WHEN("The aws is sent to IDLE mode") {
            std::this_thread::sleep_for(awsTimeProvider->getWateringSystemActivationDuration() + std::chrono::milliseconds(100));

            AND_WHEN("A shutdown request is issued") {
                auto startTime = std::chrono::steady_clock::now();
                awsUnderTest.requestShutdown();
                auto endTime = std::chrono::steady_clock::now();

                THEN("It shouldn\'t use the entire deactivation cycle to stop the aws") {
                    std::chrono::milliseconds totalWait{std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime)};
                    CHECK(totalWait <= awsTimeProvider->getWateringSystemDeactivationDuration());
                }

                THEN("The automatic watering system shouldn\'t be in run mode") {
                    CHECK_FALSE(awsUnderTest.isRunning());
                }
            }

            AND_WHEN("An abort request is issued") {
                auto startTime = std::chrono::steady_clock::now();
                awsUnderTest.emergencyAbort();
                auto endTime = std::chrono::steady_clock::now();

                THEN("It should stop the AWS under 1 second") {
                    std::chrono::milliseconds totalWait{std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime)};
                    REQUIRE(totalWait <= awsTimeProvider->getWateringSystemDeactivationDuration());
                    CHECK(totalWait < std::chrono::milliseconds{1000});
                }

                THEN("The automatic watering system shouldn\'t be in run mode") {
                    CHECK_FALSE(awsUnderTest.isRunning());
                }
            }
        }
    }
}
