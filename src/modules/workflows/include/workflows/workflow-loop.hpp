// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include "workflows/workflow.hpp"

namespace gc::workflows {

//!!
//! \brief A concept that defines the requirements for a repeat mode
//!  to be used with the WorkflowLoop class. A repeat mode must have
//!  a canRepeat() method that returns true if the workflow can be repeated
//!  and false otherwise. It must also have an iterationDone() method that
//!  is called after each iteration of the workflow.
//!
template <typename R>
concept RepeatMode = requires(R r) {
    { r.canRepeat() } -> std::convertible_to<bool>;
    { r.iterationDone() } -> std::convertible_to<void>;
};

//!!
//! \brief A class that executes a workflow in a loop, until the repeat mode
//!  is satisfied.
//!
//! \tparam R The repeat mode to use
//!
template <RepeatMode R>
class WorkflowLoop final {
public:
    using repeat_mode = R;

    explicit WorkflowLoop(Workflow& workflow, repeat_mode repeatMode) noexcept
        : m_workflow{workflow},
          m_repeatMode{std::move(repeatMode)} {}

    //!!
    //! \brief Executes the workflow in a loop, until the repeat mode is satisfied.
    //!
    //! \return true if the workflow was executed successfully, false if the workflow
    //!  returned false or an error occurred.
    //!
    [[nodiscard]] bool loopWorkflow() noexcept {
        while (m_repeatMode.canRepeat()) {
            if (!m_workflow.execute()) {
                return false;
            }

            // Notify the repeat mode so that it can update its state
            m_repeatMode.iterationDone();
        }
        return true;
    }

private:
    Workflow& m_workflow;
    repeat_mode m_repeatMode;
};

namespace repeat_modes {

//!!
//! \brief A repeat mode that repeats the workflow for the specified number of times
//!
template <std::size_t N>
class Fixed final {
public:
    [[nodiscard]] constexpr bool canRepeat() const noexcept {
        return m_count > 0;
    }

    constexpr void iterationDone() noexcept {
        --m_count;
    }

private:
    std::size_t m_count{N};
};

//!!
//! \brief A repeat mode that repeats the workflow only once
//!
using Once = Fixed<1>;

//!!
//! \brief A repeat mode that repeats the workflow for the specified number of times
//!  and the number of times is set at runtime.
//!
class Dynamic final {
public:
    constexpr explicit Dynamic(std::size_t count) noexcept : m_count(count) {}

    [[nodiscard]] constexpr bool canRepeat() const noexcept {
        return m_count > 0;
    }

    constexpr void iterationDone() noexcept {
        --m_count;
    }

    constexpr void setCount(std::size_t count) noexcept {
        m_count = count;
    }

    [[nodiscard]] constexpr std::size_t count() const noexcept {
        return m_count;
    }

private:
    std::size_t m_count;
};

//!!
//! \brief A repeat mode that repeats the workflow indefinitely, until an error occurs
//!  or the workflow execution return false.
//!
struct Indefinitely final {
    [[nodiscard]] constexpr bool canRepeat() const noexcept {
        return true;
    }

    constexpr void iterationDone() noexcept {}
};

} // namespace repeat_modes

} // namespace gc::workflows
