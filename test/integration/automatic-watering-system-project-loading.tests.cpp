// Copyright (c) 2023 Andrea Ballestrazzi

#include <automatic-watering/daily-cycle-automatic-watering-system.hpp>
#include <gc-project/project-controller.hpp>

// Test doubles
#include <gh_hal/test-doubles/hardware-access/board-digital-pin.mock.hpp>
#include <gh_log/test-doubles/logger.mock.hpp>
#include <rpi_gc/test-doubles/automatic-watering/hardware-controllers/watering-system-hardware-controller.mock.hpp>
#include <rpi_gc/test-doubles/automatic-watering/time-providers/watering-system-time-provider.mock.hpp>

#include <testing-core.hpp>

SCENARIO("AWS configuration loading integration tests",
         "[integration][AutomaticWateringSystem][Project][ProjectController]") {
    using namespace rpi_gc::gc_project;
    using namespace gc::project_management;
    using namespace rpi_gc::automatic_watering;
    using namespace std::string_literals;
    using ::testing::NiceMock;
    using ::testing::StrictMock;

    std::shared_ptr<StrictMock<gh_log::mocks::LoggerMock>> mainLoggerMock{
        std::make_shared<StrictMock<gh_log::mocks::LoggerMock>>()};
    std::shared_ptr<StrictMock<gh_log::mocks::LoggerMock>> userLoggerMock{
        std::make_shared<StrictMock<gh_log::mocks::LoggerMock>>()};

    std::unique_ptr<StrictMock<mocks::WateringSystemHardwareControllerMock>> hardwareControllerMock{
        std::make_unique<StrictMock<mocks::WateringSystemHardwareControllerMock>>()};

    std::atomic<WateringSystemHardwareController*> atomicHardwareController{
        hardwareControllerMock.get()};
    mocks::WateringSystemHardwareControllerMock& hardwareControllerMockRef{*hardwareControllerMock};

    // Create the daily cycle automatic watering system under test
    std::shared_ptr<StrictMock<mocks::WateringSystemTimeProviderMock>> timeProviderMock{
        std::make_shared<StrictMock<mocks::WateringSystemTimeProviderMock>>()};
    std::atomic<WateringSystemTimeProvider*> timeProviderAtomic{timeProviderMock.get()};
    std::mutex hardwareAccessMutex{};

    DailyCycleAutomaticWateringSystem awsUnderTest{
        std::ref(hardwareAccessMutex), mainLoggerMock, userLoggerMock,
        std::ref(atomicHardwareController), std::ref(timeProviderAtomic)};

    ProjectController projectController{};
    Project project{
        Project::time_point_type{},
        "TestProject", semver::version{1, 1, 0}
    };

    // Add the automatic watering system configuration to the project
    ProjectNode automaticWateringSystemNode{}, awsFlowNode{};

    // Configure the flow node
    awsFlowNode.addValue("isWaterValveEnabled", true);
    awsFlowNode.addValue("isWaterPumpEnabled", true);
    awsFlowNode.addValue("valvePinID", 23ull);
    awsFlowNode.addValue("pumpPinID", 26ull);
    awsFlowNode.addValue("activationTime", 600ull);
    awsFlowNode.addValue("deactivationTime", 1200ull);
    awsFlowNode.addValue("deactivationSepTime", 300ull);

    automaticWateringSystemNode.addObject("flow", std::move(awsFlowNode));
    automaticWateringSystemNode.addValue("mode", "cycled");

    project.addObject("automaticWateringSystem", std::move(automaticWateringSystemNode));
    projectController.setCurrentProject(std::move(project));
    projectController.registerProjectComponent(awsUnderTest);

    WHEN("The configuration loading for the AWS is triggered") {
        THEN("The AWS should load its configuration from the project") {
            EXPECT_CALL(hardwareControllerMockRef, setWaterValveDigitalOutputID(23, ::testing::_))
                .Times(1);
            EXPECT_CALL(hardwareControllerMockRef, setWaterPumpDigitalOutputID(26, ::testing::_))
                .Times(1);
            EXPECT_CALL(*timeProviderMock,
                        setWateringSystemActivationDuration(std::chrono::milliseconds(600)))
                .Times(1);
            EXPECT_CALL(*timeProviderMock,
                        setWateringSystemDeactivationDuration(std::chrono::milliseconds(1200)))
                .Times(1);
            EXPECT_CALL(*timeProviderMock,
                        setPumpValveDeactivationTimeSeparation(std::chrono::milliseconds(300)))
                .Times(1);

            CHECK_NOTHROW(projectController.loadProjectData());
        }
    }
}
