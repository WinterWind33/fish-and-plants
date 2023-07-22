// Copyright (c) 2023 Andrea Ballestrazzi
#include <automatic-watering/daily-cycle-automatic-watering-system.hpp>
#include <automatic-watering/time-providers/daily-cycle-aws-time-provider.hpp>
#include <gc-project/project-controller.hpp>

// Test doubles
#include <rpi_gc/test-doubles/automatic-watering/hardware-controllers/watering-system-hardware-controller.mock.hpp>
#include <gh_log/test-doubles/logger.mock.hpp>
#include <gh_hal/test-doubles/hardware-access/board-digital-pin.mock.hpp>

#include <testing-core.hpp>

SCENARIO("Daily-Cycle AWS project data saving", "[integration][AutomaticWateringSystem][Project]") {
    using namespace rpi_gc::gc_project;
    using namespace gc::project_management;
    using namespace rpi_gc::automatic_watering;
    using ::testing::NiceMock;

    std::shared_ptr<NiceMock<gh_log::mocks::LoggerMock>> mainLoggerMock{std::make_shared<NiceMock<gh_log::mocks::LoggerMock>>()};
    std::shared_ptr<NiceMock<gh_log::mocks::LoggerMock>> userLoggerMock{std::make_shared<NiceMock<gh_log::mocks::LoggerMock>>()};

    std::unique_ptr<NiceMock<mocks::WateringSystemHardwareControllerMock>> hardwareControllerMock{
        std::make_unique<NiceMock<mocks::WateringSystemHardwareControllerMock>>()
    };
    mocks::WateringSystemHardwareControllerMock& hardwareControllerMockRef{*hardwareControllerMock};
    NiceMock<gh_hal::hardware_access::mocks::BoardDigitalPinMock> valvePinMock{}, pumpPinMock{};
    ON_CALL(valvePinMock, getOffset).WillByDefault(testing::Return(23));
    ON_CALL(pumpPinMock, getOffset).WillByDefault(testing::Return(26));
    ON_CALL(hardwareControllerMockRef, getWaterValveDigitalOut).WillByDefault(::testing::Return(&valvePinMock));
    ON_CALL(hardwareControllerMockRef, getWaterPumpDigitalOut).WillByDefault(::testing::Return(&pumpPinMock));

    std::atomic<WateringSystemHardwareController*> atomicHardwareController{hardwareControllerMock.get()};

    std::shared_ptr<DailyCycleAWSTimeProvider> timeProvider{std::make_shared<DailyCycleAWSTimeProvider>()};

    std::atomic<WateringSystemTimeProvider*> timeProviderAtomic{timeProvider.get()};
    std::mutex hardwareAccessMutex{};

    DailyCycleAutomaticWateringSystem awsUnderTest{
        std::ref(hardwareAccessMutex),
        mainLoggerMock,
        userLoggerMock,
        std::ref(atomicHardwareController),
        std::ref(timeProviderAtomic)
    };

    ProjectController projectController{};
    Project project{Project::time_point_type{}, "TestProject", semver::version{1, 1, 0}};

    projectController.setCurrentProject(std::move(project));
    projectController.registerProjectComponent(awsUnderTest);

    WHEN("A project saving is triggered") {
        projectController.collectProjectData();

        THEN("The AWS should write its data to the project") {
            const Project& projectUnderTest{projectController.getCurrentProject()};

            const auto& objects{projectUnderTest.getObjects()};
            REQUIRE(objects.contains("automaticWateringSystem"));
            const auto awsObject{objects.at("automaticWateringSystem")};

            AND_THEN("The AWS object should be correct") {
                const auto& values{awsObject.getValues()};
                REQUIRE(values.contains("mode"));
                CHECK(std::get<std::string>(values.at("mode")) == "cycled");

                const auto& awsSubObjects{awsObject.getObjects()};
                REQUIRE(awsSubObjects.contains("flow"));
                const auto& awsFlowObject{awsSubObjects.at("flow")};

                AND_THEN("The flow description should be correct") {
                    const auto& awsFlowValues{awsFlowObject.getValues()};

                    REQUIRE(awsFlowValues.size() == 7);
                    REQUIRE(awsFlowValues.contains("isWaterValveEnabled"));
                    REQUIRE(awsFlowValues.contains("isWaterPumpEnabled"));
                    REQUIRE(awsFlowValues.contains("valvePinID"));
                    REQUIRE(awsFlowValues.contains("pumpPinID"));
                    REQUIRE(awsFlowValues.contains("activationTime"));
                    REQUIRE(awsFlowValues.contains("deactivationTime"));
                    REQUIRE(awsFlowValues.contains("deactivationSepTime"));

                    CHECK(std::get<bool>(awsFlowValues.at("isWaterValveEnabled")) == true);
                    CHECK(std::get<bool>(awsFlowValues.at("isWaterPumpEnabled")) == true);
                    CHECK(std::get<std::uint64_t>(awsFlowValues.at("valvePinID")) == 23);
                    CHECK(std::get<std::uint64_t>(awsFlowValues.at("pumpPinID")) == 26);
                    CHECK(std::get<std::int64_t>(awsFlowValues.at("activationTime")) == timeProvider->getWateringSystemActivationDuration().count());
                    CHECK(std::get<std::int64_t>(awsFlowValues.at("deactivationTime")) == timeProvider->getWateringSystemDeactivationDuration().count());
                    CHECK(std::get<std::int64_t>(awsFlowValues.at("deactivationSepTime")) == timeProvider->getPumpValveDeactivationTimeSeparation().count());
                }
            }
        }
    }
}
