// Copyright (C) 2023 Andrea Ballestrazzi
#ifndef TERMINAL_COMMAND_HPP
#define TERMINAL_COMMAND_HPP

#include <common/types.hpp>
#include <commands/command.hpp>

#include <gh_cmd/gh_cmd.hpp>

// C++ STL
#include <functional>
#include <type_traits>
#include <vector>
#include <string>

namespace rpi_gc {

    //!!
    //! \brief Represents a command that is being executed after the user typed it
    //!  through the command line.
    //! \tparam CharT The type of the character used to encode strings.
    template<typename CharT>
    struct TerminalCommand : public Command {
        using char_type = std::decay_t<CharT>;
        using string_type = std::basic_string<char_type>;
        using help_ostream_type = std::reference_wrapper<std::basic_ostream<char_type>>;

        ~TerminalCommand() noexcept override = default;

        //!!
        //! \brief Processes the given line tokens. The tokens should contain the command name
        //!  as the first token.
        //!
        //! \param inputTokens The options tokes to parse.
        //! \return bool Returns true if the state of the command permits a correct execution,
        //!  false otherwise.
        virtual bool processInputOptions(const std::vector<string_type>& inputTokens) noexcept = 0;

        //!!
        //! \brief Prints the help page of thic command to the given output stream.
        //!
        virtual void printHelp(help_ostream_type outputStream) const noexcept = 0;
    };

    using TerminalCommandType = TerminalCommand<CharType>;

} // namespace rpi_gc

#endif // !TERMINAL_COMMAND_HPP
