// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include "workflows/workflow.hpp"

namespace gc::workflows {

template <typename R>
concept RepeatMode = requires(R r) {
    { r.canRepeat() } -> std::convertible_to<bool>;
    { r.iterationDone() } -> std::convertible_to<void>;
};

template <RepeatMode R>
class WorkflowLoop final {
public:
    using repeat_mode = R;

    explicit WorkflowLoop(Workflow& workflow, repeat_mode repeatMode) noexcept
        : m_workflow{workflow},
          m_repeatMode{std::move(repeatMode)} {}

    [[nodiscard]] bool loopWorkflow() noexcept {
        while (m_repeatMode.canRepeat()) {
            if (!m_workflow.execute()) {
                return false;
            }

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
