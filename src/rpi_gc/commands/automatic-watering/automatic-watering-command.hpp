// Copyright (C) 2023 Andrea Ballestrazzi
#ifndef AUTOMATIC_WATERING_COMMAND_HPP
#define AUTOMATIC_WATERING_COMMAND_HPP

#include <commands/terminal-command.hpp>
#include <gh_cmd/gh_cmd.hpp>
#include <user-interface/commands-strings.hpp>

// C++ STL
#include <functional>
#include <map>

namespace rpi_gc {

//!!
//! \brief Represents the command that is executed when the user wants to activate or
//!  disable the automatic watering system.
class AutomaticWateringCommand : public TerminalCommandType {
public:
    using option_parser = gh_cmd::OptionParser<char_type>;
    using option_parser_pointer = std::unique_ptr<option_parser>;
    using ostream_ref = std::reference_wrapper<std::basic_ostream<char_type>>;
    using option_event = std::function<void(const option_parser::const_option_pointer&)>;
    using option_type = gh_cmd::CommandOption<char_type>;

    //!!
    //! \brief Construct a new Automatic Watering Command using the given output stream, where
    //!  the command execution will print out the results and the option parser used to parse
    //!  command options.
    AutomaticWateringCommand(ostream_ref outputStream, option_parser_pointer optionParser) noexcept;

    bool processInputOptions(const std::vector<string_type>& inputTokens) override;

    bool execute() noexcept override;

    void printHelp(help_ostream_type outputStream) const noexcept override;

    [[nodiscard]] constexpr name_type getName() const noexcept override {
        return strings::commands::AUTOMATIC_WATERING;
    }

    //!!
    //! \brief Registers a callback that will be executed when an option with the same name
    //!  as "optionName" is set during the command execution.
    //! \note The callback must be valid.
    void registerOptionEvent(option_type::long_name_type optionName, option_event event) noexcept;

private:
    ostream_ref m_outputStream;
    option_parser_pointer m_optionParser{};

    // Map of events that need to be triggered when an option
    // is activated.
    std::multimap<option_type::long_name_type, option_event> m_optionsEvents{};
};

} // namespace rpi_gc

#endif // !AUTOMATIC_WATERING_COMMAND_HPP
