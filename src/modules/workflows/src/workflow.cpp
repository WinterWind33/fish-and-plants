// Copyright (c) 2023 Andrea Ballestrazzi
#include "workflows/workflow.hpp"

namespace gc::workflows {

bool Workflow::execute() noexcept {
    for (auto& step : m_flow) {
        if (!step->execute()) {
            return false;
        }
    }
    return true;
}

void Workflow::addStep(std::unique_ptr<WorkflowStep> step) noexcept {
    m_flow.push_back(std::move(step));
}

void Workflow::removeStep(IdView stepId) noexcept {
    m_flow.remove_if([&stepId](const auto& step) {
        return step->id() == stepId;
    });
}

} // namespace gc::workflows
