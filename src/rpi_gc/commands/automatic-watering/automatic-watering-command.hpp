// Copyright (C) 2022 Andrea Ballestrazzi
#ifndef AUTOMATIC_WATERING_COMMAND_HPP
#define AUTOMATIC_WATERING_COMMAND_HPP

#include <user-interface/commands-strings.hpp>
#include <commands/terminal-command.hpp>
#include <gh_cmd/gh_cmd.hpp>

// C++ STL
#include <functional>

namespace rpi_gc {

    //!!
    //! \brief Represents the command that is executed when the user wants to activate or
    //!  disable the automatic watering system.
    class AutomaticWateringCommand : public TerminalCommandType {
    public:
        using option_parser = gh_cmd::OptionParser<char_type>;
        using option_parser_pointer = std::unique_ptr<option_parser>;
        using ostream_ref = std::reference_wrapper<std::basic_ostream<char_type>>;

        AutomaticWateringCommand(ostream_ref outputStream, option_parser_pointer optionParser) noexcept;

        bool processInputOptions(const std::vector<string_type>& inputTokens) noexcept override;

        bool execute() noexcept override;

        void printHelp(help_ostream_type outputStream) const noexcept override;

        constexpr name_type getName() const noexcept override {
            return strings::commands::AUTOMATIC_WATERING;
        }

    private:
        ostream_ref m_outputStream;
        option_parser_pointer m_optionParser{};
    };

} // namespace rpi_gc

#endif // !AUTOMATIC_WATERING_COMMAND_HPP
