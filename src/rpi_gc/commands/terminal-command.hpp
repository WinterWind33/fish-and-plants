// Copyright (C) 2022 Andrea Ballestrazzi
#ifndef TERMINAL_COMMAND_HPP
#define TERMINAL_COMMAND_HPP

#include <common/types.hpp>
#include <commands/command.hpp>

#include <gh_cmd/gh_cmd.hpp>

// C++ STL
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
        using non_options_vector = std::vector<string_type>;
        using unknown_options_vector = std::vector<string_type>;
        using options_vector = std::vector<std::shared_ptr<const gh_cmd::CommandOption<char_type>>>;

        ~TerminalCommand() noexcept override = default;

        //!!
        //! \brief Process the options, non options and unknown options that the user typed along
        //!  the command. Initialize the command state in order to be executed later.
        //!  If this method returns false then it's not wise to execute the command.
        //!
        //! \param options The valid options that the user typed.
        //! \param nonOptions The non-options that the user typed.
        //! \param unknowns The unknown options that the user typed.
        //! \return bool True if the state was correctly set and the command can execute, false otherwise.
        virtual bool processOptions(const options_vector& options, const non_options_vector& nonOptions,
            const unknown_options_vector& unknowns) noexcept;
    };

    using TerminalCommandType = TerminalCommand<CharType>;

} // namespace rpi_gc

#endif // !TERMINAL_COMMAND_HPP
