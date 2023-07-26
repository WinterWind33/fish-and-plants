// Copyright (C) 2023 Andrea Ballestrazzi
#ifndef BIVALENT_COMMAND_HPP
#define BIVALENT_COMMAND_HPP

#include <commands/terminal-command.hpp>
#include <gh_cmd/gh_cmd.hpp>

// C++ STL
#include <memory>

namespace rpi_gc {

//!!
//! \brief Represents a command that can be used either as a normal command
//!  or as an option for another command.
//! \tparam CharType The type of the characters to be used.
template <typename CharType>
struct BivalentCommand : public TerminalCommand<CharType> {
    using typename TerminalCommand<CharType>::char_type;
    using option_type = gh_cmd::CommandOption<char_type>;
    using option_pointer = std::shared_ptr<option_type>;

    virtual ~BivalentCommand() noexcept = default;

    //!!
    //! \brief Builds and retrieves an option starting from the command state.
    //!
    //! \return option_pointer The command represented as an option.
    virtual option_pointer getAsOption() const noexcept = 0;

    //!!
    //! \brief Executes the command but as an option of another command.
    //!
    //! \return bool True if the execution succeeded, false otherwise.
    virtual bool executeAsOption() noexcept = 0;
};

} // namespace rpi_gc

#endif // !BIVALENT_COMMAND_HPP
