// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef ABORT_COMMAND_HPP
#define ABORT_COMMAND_HPP

#include <commands/terminal-command.hpp>

#include <user-interface/commands-strings.hpp>
#include <common/types.hpp>

namespace rpi_gc::commands {

    class AbortCommand : public TerminalCommand<CharType> {
    public:
        ~AbortCommand() noexcept override = default;

        constexpr name_type getName() const noexcept override {
            return strings::commands::ABORT;
        }

        bool execute() noexcept override;

        inline bool processInputOptions(const std::vector<string_type>& inputTokens) noexcept override {
            return true;
        };

        void printHelp(help_ostream_type outputStream) const noexcept override;
    };

} // namespace rpi_gc::commands

#endif // !ABORT_COMMAND_HPP
