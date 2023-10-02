// Copyright (c) 2023 Andrea Ballestrazzi
#include "gc-project/upgraders/project-upgraders.hpp"

// C++ STL
#include <array>

namespace rpi_gc::gc_project::upgraders {

bool ProjectUpgrader_V1_1ToV1_2::tryApplyIntegrityFixes(gc::project_management::Project& project) {
    // We need to upgrade the old board digital pins representation.
    // In the V1.1.x versions the board digital pins were represented as a single value
    // (e.g. "waterPumpPinID") while in the V1.2.x versions they are represented as an array of
    // objects with their name, configurations etc.
    // We need to upgrade the old representation to the new one.

    using namespace gc::project_management;
    using namespace std::string_literals;

    // If the given project contains an "automaticWateringSystem" object we can check it and
    // upgrade it.
    if (!project.containsObject("automaticWateringSystem"s))
        return true;

    ProjectNode& awsNode{project.getObject("automaticWateringSystem"s)};
    ProjectNode& flowNode{awsNode.getObject("flow"s)};

    bool bValveEnabled{}, bPumpEnabled{};
    std::uint64_t valvePinID{}, pumpPinID{};

    // Read the old values.
    bValveEnabled = flowNode.getValue<bool>("isWaterValveEnabled"s);
    bPumpEnabled = flowNode.getValue<bool>("isWaterPumpEnabled"s);

    if (bValveEnabled) {
        valvePinID = flowNode.getValue<std::uint64_t>("valvePinID"s);
    }

    if (bPumpEnabled) {
        pumpPinID = flowNode.getValue<std::uint64_t>("pumpPinID"s);
    }

    // Now that we have read the data, we can remove the old values and add the new ones.
    flowNode.removeValue("valvePinID"s);
    flowNode.removeValue("pumpPinID"s);
    flowNode.removeValue("isWaterValveEnabled"s);
    flowNode.removeValue("isWaterPumpEnabled"s);

    // Add the new values.
    // We suppose an active-low configuration for the pins.
    std::array<ProjectNode, 2> devices{};

    devices[0]
        .addValue("name"s, "waterValve"s)
        .addValue("pinID"s, valvePinID)
        .addValue("activationState"s, "Active Low"s)
        .addValue("enabled"s, bValveEnabled);

    devices[1]
        .addValue("name"s, "waterPump"s)
        .addValue("pinID"s, pumpPinID)
        .addValue("activationState"s, "Active Low"s)
        .addValue("enabled"s, bPumpEnabled);

    flowNode.addObjectArray("devices"s, std::move(devices));

    return true;
}

} // namespace rpi_gc::gc_project::upgraders
