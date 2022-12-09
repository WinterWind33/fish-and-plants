// Copyright (C) 2022 Andrea Ballestrazzi
#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <common/types.hpp>

namespace rpi_gc {

    //! \brief Represents a command that can be executed. This is the
    //!  most basic interface for a command.
    struct Command {
        virtual ~Command() noexcept = default;

        using name_type = StringViewType;

        //!!
        //! \brief Get the name of this command
        [[nodiscard]]
        virtual name_type getName() const noexcept = 0;

        //! \brief Executes the command.
        //! \return True if the command execution went without errors, false
        //!  otherwise.
        virtual bool execute() noexcept = 0;
    };

} // namespace rpi_gc

#endif // !COMMAND_HPP
