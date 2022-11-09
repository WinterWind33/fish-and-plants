// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef HELP_COMMAND_HPP
#define HELP_COMMAND_HPP

#include <common/types.hpp>
#include <commands/terminal-command.hpp>
#include <gh_cmd/gh_cmd.hpp>

// C++ STL
#include <functional>
#include <map>
#include <ostream>

namespace rpi_gc {

    class HelpCommand : public TerminalCommandType {
    public:
        using option_parser = gh_cmd::OptionParser<CharType>;
        using option_parser_ref = std::reference_wrapper<option_parser>;
        using option_parsers_map = std::map<name_type, option_parser_ref>;
        using ostream_ref = std::reference_wrapper<std::ostream>;

        HelpCommand(ostream_ref outputStream, option_parsers_map optionParsers) noexcept;
        ~HelpCommand() noexcept override = default;

        constexpr name_type getName() const noexcept override { return "help"; }

        bool processOptions(const options_vector& options,
            const non_options_vector& nonOptions, const unknown_options_vector& unknown) noexcept override;

        bool execute() noexcept override;

    private:
        option_parsers_map m_optionParsers{};
        ostream_ref m_outputStream;

        void print_header() noexcept;
        void print_description() noexcept;
    };

} // namespace rpi_gc

#endif // !HELP_COMMAND_HPP
