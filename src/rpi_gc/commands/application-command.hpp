// Copyright (C) 2023 Andrea Ballestrazzi
#ifndef APPLICATION_HELP_COMMAND_HPP
#define APPLICATION_HELP_COMMAND_HPP

#include <commands/terminal-command.hpp>
#include <user-interface/commands-strings.hpp>
#include <commands/bivalent-command.hpp>

// C++ STL
#include <functional>
#include <map>
#include <memory>

namespace rpi_gc {

    class ApplicationCommand : public TerminalCommandType {
    public:
        using ostream_ref = std::reference_wrapper<OutputStream>;
        using option_parser_ref = std::reference_wrapper<gh_cmd::OptionParser<char_type>>;
        using option_type = gh_cmd::CommandOption<CharType>;
        using bivalent_command_ref = std::reference_wrapper<BivalentCommand<char_type>>;

        ApplicationCommand(ostream_ref outputStream, option_parser_ref optionParser) noexcept;

        bool processInputOptions(const std::vector<string_type>& options) override;

        bool execute() noexcept override;

        [[nodiscard]] constexpr name_type getName() const noexcept override {
            return name_type{strings::commands::APPLICATION};
        }

        void addBivalentCommand(bivalent_command_ref bivalentCommand) noexcept;

        void printHelp(help_ostream_type outputStream) const noexcept override {}

    private:
        ostream_ref m_outputStream;
        option_parser_ref m_optionParser;

        std::map<option_type::long_name_type, bivalent_command_ref> m_bivalentCommands{};
    };

} // namespace rpi_gc

#endif // !APPLICATION_HELP_COMMAND_HPP
