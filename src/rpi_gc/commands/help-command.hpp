// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

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
    class HelpCommand final : public BivalentCommand<CharType> {
    public:
        using ostream_ref = std::reference_wrapper<OutputStream>;
        using terminal_command_const_ref = std::reference_wrapper<const TerminalCommandType>;

        explicit HelpCommand(ostream_ref outputStream, std::vector<terminal_command_const_ref> commands) noexcept;
        ~HelpCommand() noexcept override = default;

        [[nodiscard]] constexpr name_type getName() const noexcept override { return strings::commands::HELP; }

        constexpr bool processInputOptions(const std::vector<string_type>& options) override { return true; }

        //!!
        //! \brief Prints the main help page printing out the help pages of all the
        //!  commands implemented by the application, asking them to the option parsers map.
        //! \return True.
        bool execute() noexcept override;

        bool executeAsOption() noexcept override;

        [[nodiscard]] option_pointer getAsOption() const noexcept override { return m_asOption; }

        void setApplicationHelp(string_type helpString) noexcept {
            m_applicationHelp = std::move(helpString);
        }

        void printHelp(help_ostream_type outputStream) const noexcept override;

    private:
        ostream_ref m_outputStream;
        string_type m_applicationHelp{};
        option_pointer m_asOption{};

        std::vector<terminal_command_const_ref> m_terminalCommands{};

        [[nodiscard]]
        static std::string get_app_version() noexcept;

        void print_header() noexcept;
        void print_description() noexcept;
    };

} // namespace rpi_gc
