// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef VERSION_COMMAND_HPP
#define VERSION_COMMAND_HPP

#include <common/types.hpp>
#include <commands/terminal-command.hpp>

// C++ STL
#include <functional>
#include <ostream>

namespace rpi_gc {

    //!!
    //! \brief Represents the Version command, i.e. the command executed when the user
    //!  types "version" in the app home prompt.
    class VersionCommand : public TerminalCommand<CharType> {
    public:
        using ostream_ref = std::reference_wrapper<std::ostream>;

        explicit VersionCommand(ostream_ref outputStream) noexcept;
        ~VersionCommand() noexcept override = default;

        constexpr name_type getName() const noexcept override { return name_type{"version"}; }

        //!!
        //! \brief Does nothing. Note: the options vector must be empty, the version command doesn't
        //!  accept any option.
        //! \return True
        bool processOptions(const options_vector& options,
            const non_options_vector& nonOptions, const unknown_options_vector& unknown) noexcept override;

        //!!
        //! \brief Prints the name of the application and its version.
        //! \return True.
        bool execute() noexcept override;

    private:
        ostream_ref m_outputStream;
    };

} // namespace rpi_gc

#endif // !VERSION_COMMAND_HPP
