// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// C++ STL
#include <functional>
#include <list>
#include <memory>
#include <string>

namespace gc::workflows {

using IdType = std::string;
using IdView = std::string_view;

//!!
//! \brief A workflow step is a task that can be executed in a workflow. This is a unit
//!  of work that can't be split into smaller tasks and cannot be executed in parallel.
//!
struct WorkflowStep {
    using id_type = IdType;

    virtual ~WorkflowStep() noexcept = default;

    //!!
    //! \brief Execute the task. Should not throw any exception.
    //!
    //! \return True if the task has been executed successfully, false otherwise
    //!
    [[nodiscard]] virtual bool execute() noexcept = 0;

    //!!
    //! \brief Get the name of the task
    //!
    //! \return The name of the task
    //!
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

namespace steps {

//!!
//! \brief A workflow step that executes a function
//!
class Function final : public WorkflowStep {
public:
    explicit Function(id_type stepId, std::function<bool()> function) noexcept
        : m_id{std::move(stepId)},
          m_function{std::move(function)} {}

    [[nodiscard]] bool execute() noexcept override {
        return m_function();
    }

    [[nodiscard]] id_type id() const noexcept override {
        return m_id;
    }

private:
    id_type m_id;
    std::function<bool()> m_function;
};

} // namespace steps

} // namespace gc::workflows
