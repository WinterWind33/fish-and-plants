// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <commands/terminal-command.hpp>
#include <user-interface/commands-strings.hpp>

namespace rpi_gc::commands {

    class StatusCommand final : public TerminalCommandType {
    public:

        //!!
        //! \brief Gets the name of this command. Resolves to "status".
        [[nodiscard]] constexpr name_type getName() const noexcept override {
            return strings::commands::STATUS;
        }
    };

} // namespace rpi_gc::commands
