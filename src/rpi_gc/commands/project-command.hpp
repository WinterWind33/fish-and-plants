// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <commands/terminal-command.hpp>

#include <user-interface/commands-strings.hpp>

// C++ STL
#include <vector>

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

        [[nodiscard]]
        bool execute() noexcept override;

        [[nodiscard]]
        bool processInputOptions(const std::vector<string_type>& inputTokes) noexcept override;

        void printHelp(help_ostream_type outputStream) const noexcept override;
    };

} // namespace rpi_gc::commands
