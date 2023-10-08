// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include "workflows/workflow.hpp"

namespace gc::workflows {

template <typename R>
concept RepeatMode = requires(R r) {
    { r.canRepeat() } -> std::convertible_to<bool>;
};

template <RepeatMode R>
class WorkflowLoop final {
public:
    using repeat_mode = R;

    explicit WorkflowLoop(Workflow& workflow, repeat_mode repeatMode) noexcept
        : m_workflow(workflow),
          m_repeatMode(std::move(repeatMode)) {}

    [[nodiscard]] bool loopWorkflow() noexcept {
        while (m_repeatMode.canRepeat()) {
            if (!m_workflow.execute()) {
                return false;
            }
        }
        return true;
    }

private:
    Workflow& m_workflow;
    repeat_mode m_repeatMode;
};

} // namespace gc::workflows
