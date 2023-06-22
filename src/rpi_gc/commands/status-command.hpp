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

        bool execute() noexcept override;

        [[nodiscard]]
        bool processInputOptions(const std::vector<string_type>& inputTockens) noexcept override;

        void printHelp(help_ostream_type outputStream) const noexcept override;
    };

} // namespace rpi_gc::commands
