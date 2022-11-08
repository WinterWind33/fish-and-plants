// Copyright (C) 2022 Andrea Ballestrazzi
#ifndef APPLICATION_HELP_COMMAND_HPP
#define APPLICATION_HELP_COMMAND_HPP

#include <commands/terminal-command.hpp>
#include <gh_cmd/gh_cmd.hpp>

// C++ STL
#include <functional>
#include <ostream>
#include <map>
#include <memory>

namespace rpi_gc {

    class ApplicationCommand : public TerminalCommandType {
    public:
        using ostream_ref = std::reference_wrapper<std::basic_ostream<char_type>>;
        using option_parser_ref = std::reference_wrapper<gh_cmd::OptionParser<char_type>>;
        using option_type = gh_cmd::CommandOption<CharType>;
        using option_pointer = std::shared_ptr<option_type>;

        ApplicationCommand(ostream_ref outputStream, option_parser_ref optionParser) noexcept;

        bool processOptions(const options_vector& options, const non_options_vector& nonOptions,
            const unknown_options_vector& unknowns) noexcept override;

        bool execute() noexcept override;

        constexpr name_type getName() const noexcept override {
            return name_type{"rpi_gc"};
        }

    private:
        ostream_ref m_outputStream;
        option_parser_ref m_optionParser;

        std::map<option_type::short_name_type, std::function<void()>> m_optionsCallbacks{};

        void print_help() noexcept;
        void print_version() noexcept;
    };

} // namespace rpi_gc

#endif // !APPLICATION_HELP_COMMAND_HPP