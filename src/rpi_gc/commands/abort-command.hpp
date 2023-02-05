// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef ABORT_COMMAND_HPP
#define ABORT_COMMAND_HPP

#include <abort-system/emergency-stoppable-system.hpp>

#include <commands/terminal-command.hpp>

#include <user-interface/commands-strings.hpp>
#include <common/types.hpp>

#include <gh_log/logger.hpp>

// C++ STL
#include <memory>
#include <vector>

namespace rpi_gc::commands {

    //!!
    //! \brief Represents the "abort" command, i.e. the command that performs an
    //!  emergency stop on the automatic systems that are running in the controller.
    class AbortCommand : public TerminalCommand<CharType> {
    public:
        using logger_pointer = std::shared_ptr<gh_log::Logger>;
        using emergency_stoppable_system = rpi_gc::abort_system::EmergencyStoppableSystem;
        using emergency_stoppable_system_pointer = std::shared_ptr<emergency_stoppable_system>;

        ~AbortCommand() noexcept override = default;

        //!!
        //! \brief Construct a new Abort Command object taking the specified main logger and the abortable
        //!  systems that will be aborted during the "execute()" command.
        explicit AbortCommand(logger_pointer mainLogger, std::vector<emergency_stoppable_system_pointer> systems) noexcept;

        constexpr name_type getName() const noexcept override {
            return strings::commands::ABORT;
        }

        //!!
        //! \brief For each abortable system registered in the command, it calls emergencyAbort() and wait
        //!  for each abort to end.
        bool execute() noexcept override;

        inline bool processInputOptions(const std::vector<string_type>& inputTokens) noexcept override {
            return true;
        };

        void printHelp(help_ostream_type outputStream) const noexcept override;

    private:
        logger_pointer m_mainLogger{};
        std::vector<emergency_stoppable_system_pointer> m_stoppableSystems{};

        [[nodiscard]]
        StringType static format_log_message(StringViewType message) noexcept;
    };

} // namespace rpi_gc::commands

#endif // !ABORT_COMMAND_HPP
