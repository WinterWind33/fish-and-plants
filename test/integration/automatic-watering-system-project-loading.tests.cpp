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
        "TestProject", semver::version{1, 2, 0}
    };

    // Add the automatic watering system configuration to the project
    ProjectNode automaticWateringSystemNode{}, awsFlowNode{};

    // Configure the flow node
    awsFlowNode.addValue("activationTime", 600ull);
    awsFlowNode.addValue("deactivationTime", 1200ull);
    awsFlowNode.addValue("deactivationSepTime", 300ull);

    // We add two devices to the flow node inside an object array.
    // The first device is the water valve, the second is the water pump.
    std::array<ProjectNode, 2> devices{};
    devices[0]
        .addValue("name"s, "waterValve"s)
        .addValue("pinID"s, 23ull)
        .addValue("activationState"s, "Active Low"s)
        .addValue("enabled"s, true);

    devices[1]
        .addValue("name"s, "waterPump"s)
        .addValue("pinID"s, 26ull)
        .addValue("activationState"s, "Active High"s)
        .addValue("enabled"s, true);

    awsFlowNode.addObjectArray("devices", std::move(devices));

    automaticWateringSystemNode.addObject("flow", std::move(awsFlowNode));
    automaticWateringSystemNode.addValue("mode", "cycled");

    project.addObject("automaticWateringSystem", std::move(automaticWateringSystemNode));
    projectController.setCurrentProject(std::move(project));
    projectController.registerProjectComponent(awsUnderTest);

    WHEN("The configuration loading for the AWS is triggered") {
        THEN("The AWS should load its configuration from the project") {
            EXPECT_CALL(hardwareControllerMockRef,
                        setWaterValveDigitalOutputID(
                            23, WateringSystemHardwareController::activation_state::ActiveLow))
                .Times(1);
            EXPECT_CALL(hardwareControllerMockRef,
                        setWaterPumpDigitalOutputID(
                            26, WateringSystemHardwareController::activation_state::ActiveHigh))
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
