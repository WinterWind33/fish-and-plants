// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <abort-system/emergency-stoppable-system.hpp>

#include <commands/terminal-command.hpp>

#include <user-interface/commands-strings.hpp>
#include <common/types.hpp>
#include <gh_cmd/gh_cmd.hpp>
#include <gh_log/logger.hpp>

// C++ STL
#include <memory>
#include <vector>

namespace rpi_gc::commands {

    //!!
    //! \brief Represents the "abort" command, i.e. the command that performs an
    //!  emergency stop on the automatic systems that are running in the controller.
    class AbortCommand final : public TerminalCommand<CharType> {
    public:
        using option_parser = gh_cmd::OptionParser<char_type>;
        using option_parser_pointer = std::unique_ptr<option_parser>;
        using logger_pointer = std::shared_ptr<gh_log::Logger>;
        using emergency_stoppable_system = rpi_gc::abort_system::EmergencyStoppableSystem;
        using emergency_stoppable_system_pointer = std::shared_ptr<emergency_stoppable_system>;

        ~AbortCommand() noexcept override = default;

        //!!
        //! \brief Construct a new Abort Command object taking the specified main logger and the abortable
        //!  systems that will be aborted during the "execute()" command.
        explicit AbortCommand(logger_pointer mainLogger, std::vector<emergency_stoppable_system_pointer> systems, option_parser_pointer optionParser) noexcept;

        [[nodiscard]] constexpr name_type getName() const noexcept override {
            return strings::commands::ABORT;
        }

        //!!
        //! \brief For each abortable system registered in the command, it calls emergencyAbort() and wait
        //!  for each abort to end.
        bool execute() noexcept override;

        bool processInputOptions(const std::vector<string_type>& inputTokens) override;

        void printHelp(help_ostream_type outputStream) const noexcept override;

    private:
        logger_pointer m_mainLogger{};
        option_parser_pointer m_optionParser{};

        std::vector<emergency_stoppable_system_pointer> m_stoppableSystems{};

        [[nodiscard]]
        StringType static format_log_message(StringViewType message) noexcept;
    };

} // namespace rpi_gc::commands
