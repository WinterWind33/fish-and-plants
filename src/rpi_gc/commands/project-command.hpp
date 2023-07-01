// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <commands/terminal-command.hpp>

#include <user-interface/commands-strings.hpp>

namespace rpi_gc::commands {

    class ProjectCommand final : public TerminalCommandType {
    public:

        //!!
        //! \brief Retrieves the name of this command: project.
        //!
        [[nodiscard]]
        constexpr name_type getName() const noexcept override {
            return strings::commands::PROJECT;
        }
    };

} // namespace rpi_gc::commands
