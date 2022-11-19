// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef HELP_COMMAND_HPP
#define HELP_COMMAND_HPP

#include <common/types.hpp>
#include <commands/bivalent-command.hpp>
#include <user-interface/commands-strings.hpp>

// C++ STL
#include <functional>
#include <map>

namespace rpi_gc {

    //!!
    //! \brief Represents the Help command, i.e. the command executed when the user types "help" in the
    //!  app home prompt.
    class HelpCommand : public BivalentCommand<CharType> {
    public:
        using ostream_ref = std::reference_wrapper<OutputStream>;

        HelpCommand(ostream_ref outputStream, std::vector<TerminalCommandType*> commands) noexcept;
        ~HelpCommand() noexcept override = default;

        constexpr name_type getName() const noexcept override { return strings::commands::HELP; }

        constexpr bool processInputOptions(const std::vector<string_type>& options) noexcept override { return true; }

        //!!
        //! \brief Prints the main help page printing out the help pages of all the
        //!  commands implemented by the application, asking them to the option parsers map.
        //! \return True.
        bool execute() noexcept override;

        bool executeAsOption() noexcept override;

        option_pointer getAsOption() const noexcept override { return m_asOption; }

        void setApplicationHelp(string_type helpString) noexcept {
            m_applicationHelp = std::move(helpString);
        }

        void printHelp(help_ostream_type outputStream) const noexcept override;

    private:
        ostream_ref m_outputStream;
        string_type m_applicationHelp{};
        option_pointer m_asOption{};

        std::vector<std::string> m_commandsHelpPages{};

        void print_header() noexcept;
        void print_description() noexcept;
    };

} // namespace rpi_gc

#endif // !HELP_COMMAND_HPP
