// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// C++ STL
#include <list>
#include <memory>
#include <string>

namespace gc::workflows {

using IdType = std::string;
using IdView = std::string_view;

struct WorkflowStep {
    using id_type = IdType;

    virtual ~WorkflowStep() noexcept = default;

    [[nodiscard]] virtual bool execute() noexcept = 0;
    [[nodiscard]] virtual id_type id() const noexcept = 0;
};

//!!
//! \brief A workflow is a set of tasks that can be executed in parallel or in sequence.
//!
class Workflow final {
public:
    using id_type = IdType;
    using flow_type = std::list<std::unique_ptr<WorkflowStep>>;

    explicit Workflow(id_type workflowId) noexcept : m_id(std::move(workflowId)) {}

    //!!
    //! \brief Construct a workflow with a name and a flow of tasks. The tasks
    //!  are executed sequentially in the order defined by iterating the flow
    //!  list forward.
    //!
    //! \param[in] workflowId The id of the workflow
    //! \param[in] workflowFlow The flow of tasks
    //!
    explicit Workflow(id_type workflowId, flow_type workflowFlow) noexcept
        : m_id(std::move(workflowId)),
          m_flow(std::move(workflowFlow)) {}

    //!!
    //! \brief Execute the workflow
    //!
    //! \return True if the workflow has been executed successfully, false otherwise
    //!
    [[nodiscard]] bool execute() noexcept;

    //!!
    //! \brief Add a task to the workflow
    //!
    //! \param[in] step The task to add
    //!
    void addStep(std::unique_ptr<WorkflowStep> step) noexcept;

    //!!
    //! \brief Remove a task from the workflow
    //!
    //! \param[in] stepId The id of the task to remove
    //!
    void removeStep(IdView stepId) noexcept;

    // Getters

    //!!
    //! Checks wheter the workflow is empty or not
    //!
    //! \return True if the workflow is empty, false otherwise
    //!
    [[nodiscard]] bool empty() const noexcept {
        return m_flow.empty();
    }

    //!!
    //! \brief Get the name of the workflow
    //!
    //! \return The name of the workflow
    //!
    [[nodiscard]] const id_type& id() const noexcept {
        return m_id;
    }

private:
    id_type m_id;
    flow_type m_flow{};
};

} // namespace gc::workflows
