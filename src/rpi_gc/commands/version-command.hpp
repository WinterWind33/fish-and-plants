// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef VERSION_COMMAND_HPP
#define VERSION_COMMAND_HPP

#include <common/types.hpp>
#include <commands/bivalent-command.hpp>
#include <user-interface/commands-strings.hpp>

// C++ STL
#include <functional>

namespace rpi_gc {

    //!!
    //! \brief Represents the Version command, i.e. the command executed when the user
    //!  types "version" in the app home prompt.
    class VersionCommand : public BivalentCommand<CharType> {
    public:
        using ostream_ref = std::reference_wrapper<OutputStream>;

        explicit VersionCommand(ostream_ref outputStream) noexcept;
        ~VersionCommand() noexcept override = default;

        constexpr name_type getName() const noexcept override { return strings::commands::VERSION; }

        constexpr bool processInputOptions(const std::vector<string_type>& options) noexcept override { return true; }

        //!!
        //! \brief Prints the name of the application and its version.
        //! \return True.
        bool execute() noexcept override;

        bool executeAsOption() noexcept override {
            execute();

            // We don't want to continue with the application if the user
            // requrested the application help.
            return false;
        }

        option_pointer getAsOption() const noexcept { return m_asOption; }

        void printHelp(help_ostream_type outputStream) const noexcept override;

    private:
        ostream_ref m_outputStream;
        option_pointer m_asOption{};
    };

} // namespace rpi_gc

#endif // !VERSION_COMMAND_HPP
